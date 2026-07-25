#ifndef CURRICULUM_H
#define CURRICULUM_H

#include <QString>

class Curriculum
{
public:
    Curriculum();

    Curriculum(const QString& program,
               int year,
               int semester,
               const QString& code,
               const QString& name,
               int credit);

    QString getProgram() const;
    int getYear() const;
    int getSemester() const;
    QString getCode() const;
    QString getName() const;
    int getCredit() const;

private:
    QString program;
    int year;
    int semester;
    QString code;
    QString name;
    int credit;
};

#endif