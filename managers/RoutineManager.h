#ifndef ROUTINEMANAGER_H
#define ROUTINEMANAGER_H

#include <QList>
#include <QString>

#include "../models/Routine.h"

class RoutineManager
{
private:

    QList<Routine> routines;


public:

    // Constructor
    RoutineManager();


    // Add routine
    void addRoutine(const Routine& routine);


    // Remove routine using course code
    bool removeRoutine(const QString& courseCode);


    // Update routine
    bool updateRoutine(const QString& courseCode,
                       const Routine& updatedRoutine);


    // Return all routines
    QList<Routine> getAllRoutines() const;



    // Search functions

    QList<Routine> findByProgram(
        const QString& program) const;


    QList<Routine> findByYear(
        int year) const;


    QList<Routine> findBySemester(
        int semester) const;


    QList<Routine> findBySection(
        const QString& section) const;


    QList<Routine> findByDay(
        const QString& day) const;


    QList<Routine> findByCourseCode(
        const QString& courseCode) const;



    // File operations

    bool loadFromFile(
        const QString& filename);


    bool saveToFile(
        const QString& filename) const;



    // Utility

    void clear();

    int count() const;

};


#endif // ROUTINEMANAGER_H