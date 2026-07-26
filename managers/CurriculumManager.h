#ifndef CURRICULUMMANAGER_H
#define CURRICULUMMANAGER_H

#include <QString>
#include <QList>

#include "../models/Curriculum.h"

class CourseResolver;

// Owns the curriculum data (which course codes belong to which program,
// year, and semester) and answers "show me the course catalogue/curriculum
// for X" queries.
//
// Like RoutineManager, this is structured data — searched by field
// (program), never TextMatcher — and it depends on CourseResolver, not
// CourseManager directly, to look up each course's display name. That
// keeps "no manager calls another manager" intact while still showing
// human-readable course names in the output.
//
// No WebCrawler fallback: an unrecognized program means the query needs a
// valid program name, not a web search — same reasoning as RoutineManager.
class CurriculumManager: public InformationManager
{
public:
    // courseResolver may be nullptr (e.g. unit tests) — findAnswer() then
    // shows course codes only, with no course name lookup.
    explicit CurriculumManager(CourseResolver* courseResolver = nullptr);

    bool loadCurriculum(const QString& filename);

    QList<Curriculum> getAll() const;
    QList<Curriculum> getByProgram(const QString& program) const;

    // Extracts the program referenced in `query` (via ProgramCodeUtils, so
    // it recognizes both short codes like "CE" and SynonymMapper's
    // expanded full names like "computer engineering"), then returns the
    // full formatted curriculum for that program — every year and
    // semester, in order, with course codes and (where known) names.
    QString findAnswer(const QString& query) const override;

private:
    QString formatCurriculum(
        const QString& program,
        const QList<Curriculum>& entries) const;

    QList<Curriculum> entries;
    CourseResolver* courseResolver;
};

#endif // CURRICULUMMANAGER_H