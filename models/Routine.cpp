#include "Routine.h"

// Default Constructor
Routine::Routine()
{
}

// Parameterized Constructor
Routine::Routine(const QString& program,
                 int year,
                 int semester,
                 const QString& section,
                 const QString& day,
                 const QString& time,
                 const QString& courseCode,
                 const QString& venue)
    : program(program),
    year(year),
    semester(semester),
    section(section),
    day(day),
    time(time),
    courseCode(courseCode),
    venue(venue)
{
}

// Getters

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

// Setters

void Routine::setProgram(const QString& program)
{
    this->program = program;
}

void Routine::setYear(int year)
{
    this->year = year;
}

void Routine::setSemester(int semester)
{
    this->semester = semester;
}

void Routine::setSection(const QString& section)
{
    this->section = section;
}

void Routine::setDay(const QString& day)
{
    this->day = day;
}

void Routine::setTime(const QString& time)
{
    this->time = time;
}

void Routine::setCourseCode(const QString& courseCode)
{
    this->courseCode = courseCode;
}

void Routine::setVenue(const QString& venue)
{
    this->venue = venue;
}