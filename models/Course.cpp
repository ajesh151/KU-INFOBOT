#include "Course.h"

// Default Constructor
Course::Course()
{
}

// Parameterized Constructor
Course::Course(const QString& code,
               const QString& name,
               int credits)
    : code(code),
      name(name),
      credits(credits)
{
}

// Getters

QString Course::getCode() const
{
    return code;
}

QString Course::getName() const
{
    return name;
}

int Course::getCredits() const
{
    return credits;
}

// Setters

void Course::setCode(const QString& code)
{
    this->code = code;
}

void Course::setName(const QString& name)
{
    this->name = name;
}

void Course::setCredits(int credits)
{
    this->credits = credits;
}