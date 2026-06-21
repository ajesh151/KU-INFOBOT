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

    void setProgram(const QString& program);
    void setYear(int year);
    void setSemester(int semester);
    void setSection(const QString& section);
    void setDay(const QString& day);
    void setTime(const QString& time);
    void setCourseCode(const QString& courseCode);
    void setVenue(const QString& venue);

    // Human-readable single-line representation of this entry, e.g.
    // "Monday 9:00-11:00 — MATH104 @ 9-301(Graduate Room) [CE, Year 1, Sem 2, Section A]"
    QString toString() const;
};

#endif // ROUTINE_H