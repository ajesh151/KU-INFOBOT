#include "RoutineManager.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

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