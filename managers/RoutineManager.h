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
    RoutineManager();

    bool loadRoutines(const QString& filename);

    QList<Routine> getAllRoutines() const;

    QList<Routine> getByProgram(const QString& program) const;

    QList<Routine> getBySemester(int semester) const;

    QList<Routine> getByProgramAndSemester(
        const QString& program,
        int semester) const;

    QList<Routine> getByCourseCode(
        const QString& courseCode) const;

    int size() const;
};

#endif // ROUTINEMANAGER_H