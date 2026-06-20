#include "RoutineManager.h"
#include <QFile>
#include <QTextStream>

RoutineManager::RoutineManager()
{

}
// Add routine

void RoutineManager::addRoutine(const Routine& routine)
{
    routines.append(routine);
}

// Remove routine

bool RoutineManager::removeRoutine(const QString& courseCode)
{

    for(int i=0;i<routines.size();i++)
    {
        if(routines[i].getCourseCode()==courseCode)
        {
            routines.removeAt(i);
            return true;
        }
    }

    return false;
}
// Update routine

bool RoutineManager::updateRoutine(
        const QString& courseCode,
        const Routine& updatedRoutine)
{

    for(int i=0;i<routines.size();i++)
    {
        if(routines[i].getCourseCode()==courseCode)
        {
            routines[i]=updatedRoutine;
            return true;
        }
    }

    return false;
}
// Get all

QList<Routine> RoutineManager::getAllRoutines() const
{
    return routines;
}
// Find by program

QList<Routine> RoutineManager::findByProgram(
        const QString& program) const
{

    QList<Routine> result;


    for(const Routine& r : routines)
    {
        if(r.getProgram()==program)
        {
            result.append(r);
        }
    }


    return result;
}

// Find by year

QList<Routine> RoutineManager::findByYear(
        int year) const
{

    QList<Routine> result;


    for(const Routine& r : routines)
    {
        if(r.getYear()==year)
        {
            result.append(r);
        }
    }


    return result;
}
// Find by semester

QList<Routine> RoutineManager::findBySemester(
        int semester) const
{

    QList<Routine> result;


    for(const Routine& r : routines)
    {
        if(r.getSemester()==semester)
        {
            result.append(r);
        }
    }


    return result;
}
// Find by section

QList<Routine> RoutineManager::findBySection(
        const QString& section) const
{

    QList<Routine> result;


    for(const Routine& r : routines)
    {
        if(r.getSection()==section)
        {
            result.append(r);
        }
    }


    return result;
}
// Find by day

QList<Routine> RoutineManager::findByDay(
        const QString& day) const
{

    QList<Routine> result;


    for(const Routine& r : routines)
    {
        if(r.getDay()==day)
        {
            result.append(r);
        }
    }


    return result;
}
// Find by course code

QList<Routine> RoutineManager::findByCourseCode(
        const QString& courseCode) const
{

    QList<Routine> result;


    for(const Routine& r : routines)
    {
        if(r.getCourseCode()==courseCode)
        {
            result.append(r);
        }
    }


    return result;
}
// Load routines from txt file

bool RoutineManager::loadFromFile(
        const QString& filename)
{

    QFile file(filename);


    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;



    QTextStream in(&file);



    routines.clear();



    while(!in.atEnd())
    {

        QString line=in.readLine();


        QStringList data=line.split(",");



        if(data.size()!=8)
            continue;



        Routine r(
            data[0],
            data[1].toInt(),
            data[2].toInt(),
            data[3],
            data[4],
            data[5],
            data[6],
            data[7]
        );


        routines.append(r);

    }



    file.close();


    return true;
}
// Save routines

bool RoutineManager::saveToFile(
        const QString& filename) const
{

    QFile file(filename);


    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;


    QTextStream out(&file);
    for(const Routine& r : routines)
    {

        out
        << r.getProgram() << ","
        << r.getYear() << ","
        << r.getSemester() << ","
        << r.getSection() << ","
        << r.getDay() << ","
        << r.getTime() << ","
        << r.getCourseCode() << ","
        << r.getVenue()
        << "\n";

    }
    file.close();
    return true;
}

void RoutineManager::clear()
{
    routines.clear();
}

int RoutineManager::count() const
{
    return routines.size();
}