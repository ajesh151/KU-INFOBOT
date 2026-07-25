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