#include "RoutineManager.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

RoutineManager::RoutineManager()
{
}

bool RoutineManager::loadRoutines(const QString& filename)
{
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in(&file);

    routines.clear();

    while(!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if(line.isEmpty())
        {
            continue;
        }

        QStringList data = line.split("|");

        if(data.size() != 8)
        {
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

int RoutineManager::size() const
{
    return routines.size();
}