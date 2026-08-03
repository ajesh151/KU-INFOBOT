#ifndef ROUTINE_H
#define ROUTINE_H
#include <QString>

class Routine
{
private:
    QString program;
    int year;
    int semester;
    QString section;
    QString day;
    QString time;
    QString courseCode;
    QString venue;

public:
    Routine();

    Routine(const QString& program,int year,int semester,const QString& section,const QString& day,const QString& time,const QString& courseCode,const QString& venue);

    QString getProgram() const;
    int getYear() const;
    int getSemester() const;
    QString getSection() const;
    QString getDay() const;
    QString getTime() const;
    QString getCourseCode() const;
    QString getVenue() const;
    QString toString() const;
};

#endif // ROUTINE_H