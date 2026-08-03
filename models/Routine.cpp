#include "Routine.h"
Routine::Routine()
{

}

Routine::Routine(const QString& program,int year,int semester,const QString& section,const QString& day,
                 const QString& time,const QString& courseCode,const QString& venue)
    : program(program),year(year),semester(semester),section(section),day(day),time(time),courseCode(courseCode),venue(venue)
{

}

QString Routine::getProgram() const
{
    return program;
}

int Routine::getYear() const
{
    return year;
}

int Routine::getSemester() const
{
    return semester;
}

QString Routine::getSection() const
{
    return section;
}

QString Routine::getDay() const
{
    return day;
}

QString Routine::getTime() const
{
    return time;
}

QString Routine::getCourseCode() const
{
    return courseCode;
}

QString Routine::getVenue() const
{
    return venue;
}


QString Routine::toString() const
{
    QString result = QString("%1 %2 — %3 @ %4")
        .arg(day, time, courseCode, venue);

    result += QString(" [%1, Year %2").arg(program).arg(year);

    if(semester > 0)
    {
        result += QString(", Sem %1").arg(semester);
    }

    if(!section.isEmpty())
    {
        result += QString(", Section %1").arg(section);
    }

    result += "]";

    return result;
}