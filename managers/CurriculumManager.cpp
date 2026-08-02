#include "CurriculumManager.h"
#include "../utils/CourseResolver.h"
#include "../utils/ProgramCodeUtils.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <algorithm>

CurriculumManager::CurriculumManager(CourseResolver* courseResolver)
    : courseResolver(courseResolver)
{
}

bool CurriculumManager::loadCurriculum(const QString& filename)
{
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "CurriculumManager: could not open file:" << filename;
        return false;
    }

    QTextStream in(&file);

    entries.clear();

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

        if(data.size() != 4)
        {
            qWarning() << "CurriculumManager: skipping malformed line"
                       << lineNumber << "-" << data.size()
                       << "fields instead of 4:" << line;
            continue;
        }

        Curriculum entry(
            data[0].trimmed(),         // Program
            data[1].trimmed().toInt(), // Year
            data[2].trimmed().toInt(), // Semester
            data[3].trimmed()          // Course Code
            );

        entries.append(entry);
    }

    file.close();

    return true;
}

QString CurriculumManager::formatCurriculum(
    const QString& program,
    const QList<Curriculum>& entriesIn) const
{
    QList<Curriculum> sorted = entriesIn;

    // Stable sort: only reorders by year/semester, preserves each
    // semester's own course order exactly as it appears in curriculum.txt.
    std::stable_sort(sorted.begin(), sorted.end(),
                     [](const Curriculum& a, const Curriculum& b)
                     {
                         if(a.getYear() != b.getYear())
                         {
                             return a.getYear() < b.getYear();
                         }

                         return a.getSemester() < b.getSemester();
                     });

    static const QString divider(48, '=');
    static const QString subDivider(48, '-');

    QString body;
    body += divider + "\n";
    body += QString("Curriculum : %1\n").arg(program);
    body += divider + "\n\n";

    body += QString("%1 %2 %3\n")
                .arg("Code", -12)
                .arg("Course Name", -32)
                .arg("Credits");

    int currentYear = -1;
    int currentSemester = -1;

    for(const Curriculum& entry : sorted)
    {
        if(entry.getYear() != currentYear || entry.getSemester() != currentSemester)
        {
            currentYear = entry.getYear();
            currentSemester = entry.getSemester();

            body += "\n";
            body += QString("Year %1 - Semester %2\n")
                        .arg(currentYear)
                        .arg(currentSemester);
            body += subDivider + "\n";
        }

        CourseResolver::CourseInfo info = courseResolver
                                              ? courseResolver->courseInfoForCode(entry.getCourseCode())
                                              : CourseResolver::CourseInfo();

        QString name = info.name.isEmpty() ? "-" : info.name;
        QString creditsText = info.credits >= 0 ? QString::number(info.credits) : "-";

        body += QString("%1 %2 %3\n")
                    .arg(entry.getCourseCode(), -12)
                    .arg(name, -32)
                    .arg(creditsText);
    }

    return body.trimmed();
}

QString CurriculumManager::findAnswer(const QString& query) const
{
    QString program = ProgramCodeUtils::extractProgramCode(query);

    if(program.isEmpty())
    {
        return "Please specify a program (e.g. CE, CS, Civil) to view its curriculum.";
    }

    QList<Curriculum> matches;

    for(const Curriculum& entry : entries)
    {
        if(entry.getProgram().compare(program, Qt::CaseInsensitive) == 0)
        {
            matches.append(entry);
        }
    }

    if(matches.isEmpty())
    {
        return "No curriculum found for " + program + ".";
    }

    return formatCurriculum(program, matches);
}