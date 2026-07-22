#ifndef ROUTINEMANAGER_H
#define ROUTINEMANAGER_H

#include <QString>
#include <QList>

#include "../models/Routine.h"

// RoutineManager deliberately does NOT use TextMatcher: routine data is
// structured (program/year/semester/section/day/course code), so it's
// searched by field rather than free-text matching (refactoring plan,
// Phase 7). There is also no WebCrawler fallback here — there is no
// meaningful "web search" answer to a structured timetable query; if the
// fields don't match anything, the right response is to ask for more
// detail, not to crawl the web.
//
// RoutineManager never calls CourseManager. If a query names a course by
// NAME rather than by code, ResponseGenerator resolves that via
// CourseManager first and passes the resolved code into findAnswer()'s
// second parameter — this is the one piece of cross-manager coordination
// that has to live in ResponseGenerator, since no manager may call another
// manager directly.
class RoutineManager
{
public:
    RoutineManager();

    bool loadRoutines(const QString& filename);

    QList<Routine> getAllRoutines() const;
    QList<Routine> getByProgram(const QString& program) const;
    QList<Routine> getBySemester(int semester) const;
    QList<Routine> getByProgramAndSemester(const QString& program, int semester) const;
    QList<Routine> getByCourseCode(const QString& courseCode) const;

    QList<Routine> search(
        const QString& program,
        int year,
        int semester,
        const QString& section,
        const QString& day,
        const QString& courseCode) const;

    int size() const;

    // Parses program/year/semester/section/day out of free text, runs
    // search(), and returns a fully formatted answer — or a request for
    // more detail, or "no matching routine found". If `resolvedCourseCode`
    // is non-empty it's used as-is instead of trying to find a code in
    // `query` (see class comment above for why).
    QString findAnswer(
        const QString& query,
        const QString& resolvedCourseCode = QString()) const;

    // Strips routine-query grammar ("routine", "schedule", "for", "of",
    // "year", "semester", day names, etc.) out of a query, leaving behind
    // whatever is likely a course NAME. ResponseGenerator uses this to
    // build the search string it hands to CourseManager::findByName() when
    // no course code is present in the query. Public + static so it can be
    // reused without a RoutineManager instance.
    static QString stripRoutineNoiseWords(const QString& text);

private:
    QList<Routine> routines;
};

#endif // ROUTINEMANAGER_H