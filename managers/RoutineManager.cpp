#include "RoutineManager.h"
#include "../utils/CourseResolver.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>
#include <algorithm>

RoutineManager::RoutineManager(CourseResolver* courseResolver)
    : courseResolver(courseResolver)
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

// ============================================================
// Query understanding
// ============================================================

QString RoutineManager::extractProgram(const QString& upperInput) const
{
    static const QStringList knownPrograms =
        {
            "BIT","BCA","BIM","BBA","CS","CE","EEE","BE","BPH","BSC","ME"
        };

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

int RoutineManager::extractNumberNear(
    const QString& lowerText,
    const QString& keyword) const
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

QString RoutineManager::extractSection(const QString& input) const
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

QString RoutineManager::extractDay(const QString& input) const
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

// ============================================================
// Output formatting
// ============================================================

QString RoutineManager::formatTimeRange(const QString& rawTime) const
{
    // Data files may store times as "09:00-10:30" (no spaces) or already
    // spaced. Normalize to "09:00 - 10:30" either way.
    static const QRegularExpression dashPattern("\\s*-\\s*");

    QString result = rawTime;
    result.replace(dashPattern, " - ");
    return result;
}

QString RoutineManager::buildHeader(
    const QString& program,
    int year,
    int semester,
    const QString& section) const
{
    QString header;

    if(!program.isEmpty())
    {
        header += QString("Program   : %1\n").arg(program);
    }

    // Some data rows have no year/semester recorded (e.g. this dataset's
    // Year-4 entries leave semester blank, which parses as 0). Showing
    // "Semester : 0" would read as broken, not as "not applicable" — so
    // these lines only appear when there's an actual value to show.
    if(year > 0)
    {
        header += QString("Year      : %1\n").arg(year);
    }

    if(semester > 0)
    {
        header += QString("Semester  : %1\n").arg(semester);
    }

    // Section is optional — only show it if the user actually specified
    // one, rather than guessing from whichever match happened to be first.
    if(!section.isEmpty())
    {
        header += QString("Section   : %1\n").arg(section);
    }

    return header;
}

QString RoutineManager::buildScheduleBody(QList<Routine> matches) const
{
    static const QStringList dayOrder =
        {
            "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"
        };

    std::sort(matches.begin(),
              matches.end(),
              [](const Routine &a, const Routine &b)
              {
                  int dayA = dayOrder.indexOf(a.getDay());
                  int dayB = dayOrder.indexOf(b.getDay());

                  if(dayA != dayB)
                  {
                      return dayA < dayB;
                  }

                  return a.getTime() < b.getTime();
              });

    static const QString divider(40, '-');

    // If the caller didn't pin down a single section, results can span
    // several (e.g. this dataset's Year-4 courses mix section-less "core"
    // classes with elective sections E1-E4 under the same program/year).
    // Interleaving those under plain day headers with no section shown
    // would silently hide which section each row belongs to — so when
    // that happens, add a Section column instead of hiding the ambiguity.
    bool sectionsVary = false;

    if(!matches.isEmpty())
    {
        const QString firstSection = matches.first().getSection();

        for(const Routine& routine : matches)
        {
            if(routine.getSection() != firstSection)
            {
                sectionsVary = true;
                break;
            }
        }
    }

    QString body;

    if(sectionsVary)
    {
        body += QString("%1 %2 %3 %4 %5\n")
        .arg("Time", -16)
            .arg("Sec", -6)
            .arg("Code", -10)
            .arg("Course Name", -30)
            .arg("Venue");
    }

    QString currentDay;

    for(const Routine& routine : matches)
    {
        if(currentDay != routine.getDay())
        {
            if(!currentDay.isEmpty())
            {
                body += "\n";
            }

            currentDay = routine.getDay();
            body += divider + "\n";
            body += currentDay.toUpper() + "\n";
            body += divider + "\n";
        }

        QString courseName = courseResolver
                                 ? courseResolver->courseNameForCode(routine.getCourseCode())
                                 : QString();

        if(courseName.isEmpty())
        {
            courseName = "-";
        }

        if(sectionsVary)
        {
            QString sectionLabel = routine.getSection().isEmpty()
            ? QStringLiteral("-")
            : routine.getSection();

            body += QString("%1 %2 %3 %4 %5\n")
                        .arg(formatTimeRange(routine.getTime()), -16)
                        .arg(sectionLabel, -6)
                        .arg(routine.getCourseCode(), -10)
                        .arg(courseName, -30)
                        .arg(routine.getVenue());
        }
        else
        {
            body += QString("%1 %2 %3 %4\n")
            .arg(formatTimeRange(routine.getTime()), -16)
                .arg(routine.getCourseCode(), -10)
                .arg(courseName, -30)
                .arg(routine.getVenue());
        }
    }

    return body.trimmed();
}

// ============================================================
// Orchestration
// ============================================================

QString RoutineManager::findAnswer(const QString& query) const
{
    QString upperInput = query.toUpper();
    QString lowerInput = query.toLower();

    QString program = extractProgram(upperInput);
    int year = extractNumberNear(lowerInput, "year");
    int semester = extractNumberNear(lowerInput, "semester");
    QString section = extractSection(query);
    QString day = extractDay(lowerInput);

    QString courseCode;

    if(courseResolver)
    {
        CourseResolver::Resolution resolution = courseResolver->resolve(query);

        if(resolution.ambiguous)
        {
            return resolution.ambiguityMessage;
        }

        courseCode = resolution.courseCode;
    }

    if(!courseCode.isEmpty() &&
        (program.isEmpty() || year == -1 || semester == -1))
    {
        return "Please specify Program, Year and Semester along with the Course Code.";
    }

    QList<Routine> matches = search(
        program, year, semester, section, day, courseCode);

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

    // Header reflects what the user actually asked for. year/semester are
    // guaranteed non/-1 here whenever a course code was given (checked
    // above), and search() guarantees every match shares them whenever
    // they were specified. If the user didn't specify program/year/
    // semester at all (e.g. browsing by day only), fall back to the first
    // match so the header still makes sense.
    QString headerProgram = !program.isEmpty() ? program : matches.first().getProgram();
    int headerYear = year != -1 ? year : matches.first().getYear();
    int headerSemester = semester != -1 ? semester : matches.first().getSemester();

    QString response =
        buildHeader(headerProgram, headerYear, headerSemester, section) +
        "\n" +
        buildScheduleBody(matches);

    return response.trimmed();
}