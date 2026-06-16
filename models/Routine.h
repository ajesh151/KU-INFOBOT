#ifndef ROUTINE_H
#define ROUTINE_H

#include <QString>

class Routine
{
private:
    QString program;
    QString semester;
    QString day;
    QString time;
    QString subject;

public:
    Routine();
    Routine(QString program,
            QString semester,
            QString day,
            QString time,
            QString subject);

    QString getProgram() const;
    QString getSemester() const;
    QString getDay() const;
    QString getTime() const;
    QString getSubject() const;

    void setProgram(QString program);
    void setSemester(QString semester);
    void setDay(QString day);
    void setTime(QString time);
    void setSubject(QString subject);
};

#endif