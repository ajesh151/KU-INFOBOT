#include "Curriculum.h"

Curriculum::Curriculum()
{
}

Curriculum::Curriculum(
    const QString& program,
    int year,
    int semester,
    const QString& courseCode)
    : program(program),
    year(year),
    semester(semester),
    courseCode(courseCode)
{
}

QString Curriculum::getProgram() const
{
    return program;
}

int Curriculum::getYear() const
{
    return year;
}

int Curriculum::getSemester() const
{
    return semester;
}

QString Curriculum::getCourseCode() const
{
    return courseCode;
}
