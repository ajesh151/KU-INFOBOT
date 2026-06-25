#include "ResponseGenerator.h"
#include <QRegularExpression>
#include <QStringList>

namespace
{
    const QStringList knownPrograms =
    {
        "BIT","BCA","BIM","BBA","CS","CE","EEE","BE","BPH","BSC","ME"
    };

    QString extractProgram(const QString& upperInput)
    {
        for(const QString& program : knownPrograms)
        {
            QRegularExpression pattern(
                "\\b" + QRegularExpression::escape(program) + "\\b");

            if(pattern.match(upperInput).hasMatch())
            {
                return program;
            }
        }

        return QString();
    }

    int extractNumberNear(
        const QString& lowerText,
        const QString& keyword)
    {
        QRegularExpression digitBefore(
            "(\\d)(?:st|nd|rd|th)?\\s*" + keyword,
            QRegularExpression::CaseInsensitiveOption);

        auto beforeMatch = digitBefore.match(lowerText);

        if(beforeMatch.hasMatch())
        {
            return beforeMatch.captured(1).toInt();
        }

        QRegularExpression digitAfter(
            keyword + "\\s*(\\d)",
            QRegularExpression::CaseInsensitiveOption);

        auto afterMatch = digitAfter.match(lowerText);

        if(afterMatch.hasMatch())
        {
            return afterMatch.captured(1).toInt();
        }

        if(keyword == "semester")
        {
            if(lowerText.contains("first semester")) return 1;
            if(lowerText.contains("second semester")) return 2;
            if(lowerText.contains("third semester")) return 3;
            if(lowerText.contains("fourth semester")) return 4;
            if(lowerText.contains("fifth semester")) return 5;
            if(lowerText.contains("sixth semester")) return 6;
            if(lowerText.contains("seventh semester")) return 7;
            if(lowerText.contains("eighth semester")) return 8;
        }

        if(keyword == "year")
        {
            if(lowerText.contains("first year")) return 1;
            if(lowerText.contains("second year")) return 2;
            if(lowerText.contains("third year")) return 3;
            if(lowerText.contains("fourth year")) return 4;
        }

        return -1;
    }

    QString extractSection(const QString& input)
    {
        QRegularExpression pattern(
            "section\\s*([A-Za-z0-9]{1,3})\\b",
            QRegularExpression::CaseInsensitiveOption);

        auto match = pattern.match(input);

        if(match.hasMatch())
        {
            return match.captured(1).toUpper();
        }

        return QString();
    }

    QString extractDay(const QString& input)
    {
        static const QStringList days =
        {
            "monday",
            "tuesday",
            "wednesday",
            "thursday",
            "friday",
            "saturday",
            "sunday"
        };

        for(const QString& day : days)
        {
            if(input.contains(day, Qt::CaseInsensitive))
            {
                return day;
            }
        }

        return QString();
    }

    QString extractCourseCode(const QString& upperInput)
    {
        QRegularExpression pattern("[A-Z]{3,5}\\d{3}[A-Z]*");

        auto match = pattern.match(upperInput);

        if(match.hasMatch())
        {
            return match.captured(0);
        }

        return QString();
    }
}

ResponseGenerator::ResponseGenerator(
    CourseManager* courseManager,
    RoutineManager* routineManager,
    FaqManager* faqManager)
    : courseManager(courseManager),
      routineManager(routineManager),
    faqManager(faqManager)
{
}

QString ResponseGenerator::generateResponse(
        Intent intent,
        const QString& userInput) const
{
    switch(intent)
    {
    case Intent::GREETING:
        return "Hello! How can I help you today?";

    case Intent::GOODBYE:
        return "Goodbye! Have a great day.";

    case Intent::COURSE_INFO:
    {
        QString upperInput = userInput.toUpper();

        QStringList words = upperInput.split(
            QRegularExpression("\\s+"),
            Qt::SkipEmptyParts);

        for(QString word : words)
        {
            word.remove('?');
            word.remove('.');
            word.remove(',');
            word.remove(';');
            word.remove(':');

            Course course = courseManager->findByCode(word);

            if(!course.getCode().isEmpty())
            {
                return QString(
                    "Course Code : %1\n"
                    "Course Name : %2\n"
                    "Credits : %3")
                        .arg(course.getCode())
                        .arg(course.getName())
                        .arg(course.getCredits());
            }
        }

        return "Course not found.";
    }

    case Intent::ROUTINE_QUERY:
    {
        QString upperInput = userInput.toUpper();
        QString lowerInput = userInput.toLower();

        QString program = extractProgram(upperInput);
        int year = extractNumberNear(lowerInput, "year");
        int semester = extractNumberNear(lowerInput, "semester");
        QString section = extractSection(userInput);
        QString day = extractDay(lowerInput);
        QString courseCode = extractCourseCode(upperInput);

        if(!courseCode.isEmpty() &&
           (program.isEmpty() || year == -1 || semester == -1))
        {
            return "Please specify Program, Year and Semester along with the Course Code.";
        }

        QList<Routine> matches = routineManager->search(
                    program,
                    year,
                    semester,
                    section,
                    day,
                    courseCode);

        if(matches.isEmpty())
        {
            return "No matching routine found.\n\n"
                   "Please provide:\n"
                   "- Program\n"
                   "- Year\n"
                   "- Semester\n"
                   "- Section (optional)\n"
                   "- Day (optional)\n"
                   "- Course Code (optional)";
        }

        QString response =
                QString("Found %1 class(es):\n\n")
                .arg(matches.size());

        for(const Routine& routine : matches)
        {
            response += QString(
                "Program   : %1\n"
                "Year      : %2\n"
                "Semester  : %3\n"
                "Section   : %4\n"
                "Day       : %5\n"
                "Time      : %6\n"
                "Course    : %7\n"
                "Venue     : %8\n"
                "-----------------------------\n")
                    .arg(routine.getProgram())
                    .arg(routine.getYear())
                    .arg(routine.getSemester())
                    .arg(routine.getSection())
                    .arg(routine.getDay())
                    .arg(routine.getTime())
                    .arg(routine.getCourseCode())
                    .arg(routine.getVenue());
        }

        return response.trimmed();
    }

    
    case Intent::FAQ_QUERY:
    {
        return faqManager->findAnswer(userInput);
    }
/*
    case Intent::ADMISSION_QUERY:
    {
        return admissionManager->findInfo(userInput);
    }
    */

    case Intent::UNKNOWN:
    default:
        return "Sorry, I could not understand your question.";
    }
}