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
    Course(const QString& code, const QString& name, int credits);

    QString getCode() const;
    QString getName() const;
    int getCredits() const;

    void setCode(const QString& code);
    void setName(const QString& name);
    void setCredits(int credits);
};

#endif