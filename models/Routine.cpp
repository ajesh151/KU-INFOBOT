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