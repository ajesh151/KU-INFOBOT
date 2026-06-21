#include "ResponseGenerator.h"
#include <QRegularExpression>
#include <QStringList>

namespace
{
    const QStringList knownPrograms = {"CE", "CS", "BIT", "ME"};

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

    int extractNumberNear(const QString& lowerText, const QString& keyword)
    {
        // Prefer "<digit><suffix?> <keyword>" e.g. "1st year", "2nd semester"
        // so that "1st year 2nd semester" doesn't mix up the two numbers.
        QRegularExpression beforePattern(
            "(\\d)(?:st|nd|rd|th)?\\s*" + keyword,
            QRegularExpression::CaseInsensitiveOption);

        QRegularExpressionMatch beforeMatch = beforePattern.match(lowerText);

        if(beforeMatch.hasMatch())
        {
            return beforeMatch.captured(1).toInt();
        }

        // Fall back to "<keyword> <digit>" e.g. "semester 2"
        QRegularExpression afterPattern(
            keyword + "\\s*(\\d)",
            QRegularExpression::CaseInsensitiveOption);

        QRegularExpressionMatch afterMatch = afterPattern.match(lowerText);

        if(afterMatch.hasMatch())
        {
            return afterMatch.captured(1).toInt();
        }

        return -1;
    }

    QString extractSection(const QString& input)
    {
        QRegularExpression pattern(
            "section\\s*([A-Za-z0-9]{1,3})\\b",
            QRegularExpression::CaseInsensitiveOption);

        QRegularExpressionMatch match = pattern.match(input);

        if(match.hasMatch())
        {
            return match.captured(1);
        }

        return QString();
    }

    QString extractDay(const QString& input)
    {
        static const QStringList days = {
            "monday", "tuesday", "wednesday", "thursday",
            "friday", "saturday", "sunday"};

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
        QRegularExpressionMatch match = pattern.match(upperInput);

        if(match.hasMatch())
        {
            return match.captured(0);
        }

        return QString();
    }
}

ResponseGenerator::ResponseGenerator(
    CourseManager* courseManager,
    RoutineManager* routineManager/*,
    FaqManager* faqManager,
    AdmissionManager* admissionManager*/)
    : courseManager(courseManager),
    routineManager(routineManager)/*,
    faqManager(faqManager),
    admissionManager(admissionManager)*/
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

            Course course =
                courseManager->findByCode(word);

            if(!course.getCode().isEmpty())
            {
                return QString(
                           "Course Code: %1\n"
                           "Course Name: %2\n"
                           "Credits: %3")
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
        QString day = extractDay(userInput);
        QString courseCode = extractCourseCode(upperInput);

        QList<Routine> matches = routineManager->search(
            program, year, semester, section, day, courseCode);

        if(matches.isEmpty())
        {
            return "I couldn't find any routine matching that. "
                   "Try specifying program, year, "
                   "semester, section, day, or course code.";
        }

        QString response = QString(
            "Found %1 class(es):\n").arg(matches.size());

        for(const Routine& routine : matches)
        {
            response += routine.toString() + "\n";
        }

        return response.trimmed();
    }

    // case Intent::FAQ_QUERY:
    // {
    //     QString answer = faqManager.findAnswer(userInput);

    //     return answer;
    // }

    // case Intent::ADMISSION_QUERY:
    // {
    //     return admissionManager->findInfo(userInput);
    // }

    case Intent::UNKNOWN:
    default:
        return "Sorry, I could not understand your question.";
    }
}