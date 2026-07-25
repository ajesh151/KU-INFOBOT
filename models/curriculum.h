#ifndef CURRICULUM_H
#define CURRICULUM_H

#include <QString>

// One row of the academic curriculum: which course belongs to which
// program, year, and semester. This is a different concept from Routine —
// a Routine is a scheduled class instance (day/time/section/venue);
// Curriculum is the degree STRUCTURE (which subjects exist in which
// semester), independent of when or where they're actually taught.
class Curriculum
{
public:
    Curriculum();
    Curriculum(
        const QString& program,
        int year,
        int semester,
        const QString& courseCode);

    QString getProgram() const;
    int getYear() const;
    int getSemester() const;
    QString getCourseCode() const;


private:
    QString program;
    int year = 0;
    int semester = 0;
    QString courseCode;
};

#endif // CURRICULUM_H