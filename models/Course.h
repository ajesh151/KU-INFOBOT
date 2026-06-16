course.h

#ifndef COURSE_H
#define COURSE_H

#include <QString>

class Course
{
private:
    QString code;
    QString name;
    int credits;

public:
    Course();
    Course(QString code, QString name, int credits);

    QString getCode() const;
    QString getName() const;
    int getCredits() const;

    void setCode(QString code);
    void setName(QString name);
    void setCredits(int credits);
};

#endif