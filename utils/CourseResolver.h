#ifndef COURSERESOLVER_H
#define COURSERESOLVER_H

#include <QString>

class CourseManager;

// Shared service that knows how to turn free text referencing a course —
// by explicit code OR by name — into a canonical course code. This is the
// one place that understands "Programming Fundamentals" means "COMP101";
// neither RoutineManager nor ResponseGenerator need to know that mapping
// exists.
//
// RoutineManager depends on CourseResolver, NOT on CourseManager directly.
// That is what keeps "no manager calls another manager" intact while still
// letting routine queries reference a course by name instead of by code.
class CourseResolver
{
public:
    // Result of attempting to resolve a course reference in free text.
    struct Resolution
    {
        QString courseCode;        // resolved code, empty if none found
        bool ambiguous = false;    // true if multiple course names matched
        QString ambiguityMessage;  // ready-to-return text when ambiguous
    };

    explicit CourseResolver(CourseManager* courseManager);

    // Tries, in order: an explicit course code anywhere in `text`, then a
    // course NAME lookup (with routine-query grammar stripped out first)
    // via CourseManager::findByName().
    Resolution resolve(const QString& text) const;

    // Looks up the display name for a known course code (e.g. "COMP102" ->
    // "Programming Fundamentals"). Returns an empty QString if the code
    // isn't found or no CourseManager was provided. This is the other half
    // of what lets RoutineManager show course names without ever touching
    // CourseManager itself.
    QString courseNameForCode(const QString& code) const;

    // Fuller lookup used by CurriculumManager, which needs credits as well
    // as the name. name is empty and credits is -1 when the code isn't
    // found or no CourseManager was provided.
    struct CourseInfo
    {
        QString name;
        int credits = -1;
    };

    CourseInfo courseInfoForCode(const QString& code) const;

private:
    CourseManager* courseManager;
};

#endif // COURSERESOLVER_H