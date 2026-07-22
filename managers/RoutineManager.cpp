#include "RoutineManager.h"
#include "../utils/CourseCodeUtils.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>
#include <algorithm>

namespace
{
// Moved in from ResponseGenerator — this is routine-query-specific
// parsing, not routing, so it belongs here.
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
            "sunday",
            "monday",
            "tuesday",
            "wednesday",
            "thursday",
            "friday",
            "saturday"
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

QString formatRoutines(const QList<Routine>& matchesIn)
{
    QList<Routine> matches = matchesIn;

    static const QStringList dayOrder =
        {
            "Sunday",
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday"
        };

    std::sort(matches.begin(),
              matches.end(),
              [](const Routine &a,
                 const Routine &b)
              {
                  int dayA = dayOrder.indexOf(a.getDay());
                  int dayB = dayOrder.indexOf(b.getDay());

                  if(dayA != dayB)
                  {
                      return dayA < dayB;
                  }

                  return a.getTime() < b.getTime();
              });

    const Routine &first = matches.first();

    QString response =
        QString("Routine for %1-%2 (%3/%4) is:\n\n")
            .arg(first.getProgram())
            .arg(first.getSection())
            .arg(first.getYear())
            .arg(first.getSemester());

    QString currentDay;

    for(const Routine &routine : matches)
    {
        if(currentDay != routine.getDay())
        {
            if(!currentDay.isEmpty())
            {
                response += "\n";
            }

            currentDay = routine.getDay();
            response += currentDay + "\n";
        }

        response += QString("%1\t%2\t%3\n")
                        .arg(routine.getTime(), -15)
                        .arg(routine.getCourseCode(), -10)
                        .arg(routine.getVenue());
    }

    return response.trimmed();
}
}

RoutineManager::RoutineManager()
{
}

bool RoutineManager::loadRoutines(const QString& filename)
{
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "RoutineManager: could not open file:" << filename;
        return false;
    }

    QTextStream in(&file);

    routines.clear();

    int lineNumber = 0;

    while(!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        ++lineNumber;

        if(line.isEmpty())
        {
            continue;
        }

        QStringList data = line.split("|");

        if(data.size() != 8)
        {
            qWarning() << "RoutineManager: skipping malformed line"
                       << lineNumber << "-" << data.size()
                       << "fields instead of 8:" << line;
            continue;
        }

        Routine routine(
            data[0],                 // Program
            data[1].toInt(),         // Year
            data[2].toInt(),         // Semester
            data[3],                 // Section
            data[4],                 // Day
            data[5],                 // Time
            data[6],                 // Course Code
            data[7]                  // Venue
            );

        routines.append(routine);
    }

    file.close();

    return true;
}

QList<Routine> RoutineManager::getAllRoutines() const
{
    return routines;
}

QList<Routine> RoutineManager::getByProgram(
    const QString& program) const
{
    QList<Routine> result;

    for(const Routine& routine : routines)
    {
        if(routine.getProgram().compare(
                program,
                Qt::CaseInsensitive) == 0)
        {
            result.append(routine);
        }
    }

    return result;
}

QList<Routine> RoutineManager::getBySemester(
    int semester) const
{
    QList<Routine> result;

    for(const Routine& routine : routines)
    {
        if(routine.getSemester() == semester)
        {
            result.append(routine);
        }
    }

    return result;
}

QList<Routine> RoutineManager::getByProgramAndSemester(
    const QString& program,
    int semester) const
{
    QList<Routine> result;

    for(const Routine& routine : routines)
    {
        if(routine.getProgram().compare(
                program,
                Qt::CaseInsensitive) == 0
            &&
            routine.getSemester() == semester)
        {
            result.append(routine);
        }
    }

    return result;
}

QList<Routine> RoutineManager::getByCourseCode(
    const QString& courseCode) const
{
    QList<Routine> result;

    for(const Routine& routine : routines)
    {
        if(routine.getCourseCode().compare(
                courseCode,
                Qt::CaseInsensitive) == 0)
        {
            result.append(routine);
        }
    }

    return result;
}

QList<Routine> RoutineManager::search(
    const QString& program,
    int year,
    int semester,
    const QString& section,
    const QString& day,
    const QString& courseCode) const
{
    QList<Routine> result;

    for(const Routine& routine : routines)
    {
        if(!program.isEmpty() &&
            routine.getProgram().compare(program, Qt::CaseInsensitive) != 0)
        {
            continue;
        }

        if(year != -1 && routine.getYear() != year)
        {
            continue;
        }

        if(semester != -1 && routine.getSemester() != semester)
        {
            continue;
        }

        if(!section.isEmpty() &&
            routine.getSection().compare(section, Qt::CaseInsensitive) != 0)
        {
            continue;
        }

        if(!day.isEmpty() &&
            routine.getDay().compare(day, Qt::CaseInsensitive) != 0)
        {
            continue;
        }

        if(!courseCode.isEmpty() &&
            routine.getCourseCode().compare(courseCode, Qt::CaseInsensitive) != 0)
        {
            continue;
        }

        result.append(routine);
    }

    return result;
}

int RoutineManager::size() const
{
    return routines.size();
}

QString RoutineManager::stripRoutineNoiseWords(const QString& text)
{
    QString searchText = text;

    static const QStringList ignoredWords =
        {
            "routine","schedule","timetable",
            "when","what","which","show","give","display",
            "class","course","for","of","is","are",
            "me","please",
            "program","year","semester","section",
            "today","tomorrow","on"
        };

    for(const QString &word : ignoredWords)
    {
        searchText.replace(
            QRegularExpression(
                "\\b" + QRegularExpression::escape(word) + "\\b",
                QRegularExpression::CaseInsensitiveOption),
            " ");
    }

    return searchText.simplified();
}

QString RoutineManager::findAnswer(
    const QString& query,
    const QString& resolvedCourseCode) const
{
    QString upperInput = query.toUpper();
    QString lowerInput = query.toLower();

    QString program = extractProgram(upperInput);
    int year = extractNumberNear(lowerInput, "year");
    int semester = extractNumberNear(lowerInput, "semester");
    QString section = extractSection(query);
    QString day = extractDay(lowerInput);

    // Prefer a code ResponseGenerator already resolved from a course NAME
    // via CourseManager; otherwise look for an explicit code in the text.
    QString courseCode = !resolvedCourseCode.isEmpty()
                             ? resolvedCourseCode
                             : CourseCodeUtils::extractCourseCode(query);

    if(!courseCode.isEmpty() &&
        (program.isEmpty() ||
         year == -1 ||
         semester == -1))
    {
        return "Please specify Program, Year and Semester along with the Course Code.";
    }

    QList<Routine> matches = search(
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

    return formatRoutines(matches);
}