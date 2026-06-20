#ifndef ROUTINEMANAGER_H
#define ROUTINEMANAGER_H

#include <QList>
#include <QString>
#include "routine.h"

class RoutineManager
{
private:
    QList<Routine> routines;

public:
    RoutineManager();

    // Add routine
    void addRoutine(const Routine& routine);

    // Remove routine by course code
    bool removeRoutine(const QString& courseCode);

    // Update existing routine
    bool updateRoutine(const QString& courseCode, const Routine& updatedRoutine);

    // Get all routines
    QList<Routine> getAllRoutines() const;

    // Search functions
    QList<Routine> getRoutineByProgram(const QString& program) const;
    QList<Routine> getRoutineByYear(int year) const;
    QList<Routine> getRoutineBySemester(int semester) const;
    QList<Routine> getRoutineBySection(const QString& section) const;
    QList<Routine> getRoutineByDay(const QString& day) const;
    QList<Routine> getRoutineByCourseCode(const QString& courseCode) const;

    // Clear all routines
    void clearRoutines();

    // Count routines
    int getRoutineCount() const;
};

#endif // ROUTINEMANAGER_H