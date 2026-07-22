#ifndef COURSECODEUTILS_H
#define COURSECODEUTILS_H

#include <QString>

// Single shared definition of what a "course code" looks like (e.g.
// COSC101, CE204A), used by CourseManager, RoutineManager, and
// ResponseGenerator. Previously this regex was duplicated in
// IntentRecognizer and ResponseGenerator separately — now there's exactly
// one place that knows the pattern.
namespace CourseCodeUtils
{
// Returns the first course code found anywhere in `text`, uppercased,
// or an empty QString if none is present.
QString extractCourseCode(const QString& text);
}

#endif // COURSECODEUTILS_H
