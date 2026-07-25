#include "CourseResolver.h"
#include "../managers/CourseManager.h"
#include "CourseCodeUtils.h"
#include "../models/Course.h"

#include <QRegularExpression>
#include <QStringList>

namespace
{
// Strips routine-query grammar out of the text, leaving behind
// whatever is likely a course NAME. Only used internally here — this
// is exactly the kind of domain knowledge that shouldn't leak out to
// RoutineManager or ResponseGenerator.
QString stripToLikelyName(const QString& text)
{
    QString searchText = text;

    static const QStringList ignoredWords =
        {
            "routine","schedule","timetable",
            "when","what","which","show","give","display",
            "class","course","for","of","is","are",
            "me","please",
            "program","year","semester","section",
            "today","tomorrow","on"
        };

    for(const QString &word : ignoredWords)
    {
        searchText.replace(
            QRegularExpression(
                "\\b" + QRegularExpression::escape(word) + "\\b",
                QRegularExpression::CaseInsensitiveOption),
            " ");
    }

    return searchText.simplified();
}
}

CourseResolver::CourseResolver(CourseManager* courseManager)
    : courseManager(courseManager)
{
}

QString CourseResolver::courseNameForCode(const QString& code) const
{
    if(!courseManager || code.isEmpty())
    {
        return QString();
    }

    return courseManager->findByCode(code).getName();
}

CourseResolver::CourseInfo CourseResolver::courseInfoForCode(const QString& code) const
{
    CourseInfo info;

    if(!courseManager || code.isEmpty())
    {
        return info;
    }

    Course course = courseManager->findByCode(code);

    if(course.getCode().isEmpty())
    {
        // Not found — return the default empty/-1 info.
        return info;
    }

    info.name = course.getName();
    info.credits = course.getCredits();
    return info;
}

CourseResolver::Resolution CourseResolver::resolve(const QString& text) const
{
    Resolution resolution;

    // An explicit code always wins — no need to search names at all.
    QString code = CourseCodeUtils::extractCourseCode(text);

    if(!code.isEmpty())
    {
        resolution.courseCode = code;
        return resolution;
    }

    if(!courseManager)
    {
        return resolution;
    }

    QString searchText = stripToLikelyName(text);

    if(searchText.isEmpty())
    {
        return resolution;
    }

    std::vector<Course> matches = courseManager->findByName(searchText);

    if(matches.size() == 1)
    {
        resolution.courseCode = matches.front().getCode();
    }
    else if(matches.size() > 1)
    {
        resolution.ambiguous = true;

        QString message = "Multiple courses matched your query:\n\n";

        for(const Course& course : matches)
        {
            message += QString("%1 - %2\n")
            .arg(course.getCode())
                .arg(course.getName());
        }

        message += "\nPlease specify the exact course.";
        resolution.ambiguityMessage = message;
    }

    return resolution;
}