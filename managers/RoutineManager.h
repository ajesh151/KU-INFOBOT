#ifndef ROUTINEMANAGER_H
#define ROUTINEMANAGER_H

#include <QString>
#include <QList>

#include "../models/Routine.h"

class CourseResolver;

// RoutineManager owns the entire routine-query flow: parsing free text
// into fields, running the structured search (never TextMatcher — routine
// data isn't free text), sorting, and formatting the final answer.
// ResponseGenerator only calls findAnswer() and returns whatever comes
// back — it does no parsing, searching, or formatting of its own.
//
// RoutineManager still never touches CourseManager directly. It uses
// CourseResolver — which is not a manager — to resolve a course NAME to a
// code and to look up a code's display name for the output table. That
// keeps "no manager calls another manager" intact while RoutineManager
// remains the single owner of everything routine-related.
//
// No WebCrawler here by design: there's no meaningful "web search" answer
// to a structured timetable query. If the fields don't match anything, the
// right response is asking for more detail, not crawling the web.
class RoutineManager
{
public:
    // courseResolver may be nullptr (e.g. unit tests exercising only
    // code-based queries) — findAnswer() then can't resolve course NAMEs
    // or show course names in the table, only codes.
    explicit RoutineManager(CourseResolver* courseResolver = nullptr);

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

    // Parses `query`, searches, sorts, and returns the fully formatted
    // answer — or a request for more detail, or a "no match" message.
    QString findAnswer(const QString& query) const;

private:
    // --- Query understanding (previously RoutineQueryParser) ---
    // Program extraction lives in ProgramCodeUtils, not here — it has to
    // recognize both the short code and SynonymMapper's expanded full
    // name, which is shared, cross-cutting knowledge, not routine-specific.
    //
    // Year and semester are extracted TOGETHER in one pass, not by two
    // independent per-keyword searches. Two independent searches can each
    // "steal" a number that actually belongs to the other field — e.g. in
    // "year 1 semester 2", the "1" sits immediately before "semester" too,
    // so a naive standalone semester search matches "1 semester" and
    // returns 1 instead of 2. Scanning left-to-right once and consuming
    // each (number, keyword) pair as it's found makes that impossible: a
    // number used for "year" is gone from the string as far as the scan
    // is concerned by the time it looks for "semester".
    void extractYearAndSemester(
        const QString& lowerText,
        int& year,
        int& semester) const;
    QString extractSection(const QString& input) const;
    QString extractDay(const QString& input) const;

    // --- Output formatting (previously RoutineFormatter) ---
    QString formatTimeRange(const QString& rawTime) const;
    QString buildHeader(
        const QString& program,
        int year,
        int semester,
        const QString& section) const;
    QString buildScheduleBody(QList<Routine> matches) const;

    QList<Routine> routines;
    CourseResolver* courseResolver;
};

#endif // ROUTINEMANAGER_H