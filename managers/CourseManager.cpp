#include "CourseManager.h"
#include "../utils/CourseCodeUtils.h"
#include "../utils/TextMatcher.h"
#include "../crawler/WebCrawler.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

CourseManager::CourseManager(WebCrawler* webCrawler)
    : webCrawler(webCrawler)
{
}

bool CourseManager::loadCourses(const QString& filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    courses.clear();

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
        {
            continue;
        }

        QStringList parts = line.split('|');

        if (parts.size() != 3)
        {
            continue;
        }

        Course course(
            parts[0].trimmed(),          // Code
            parts[1].trimmed(),          // Name
            parts[2].trimmed().toInt()   // Credits
            );

        courses.push_back(course);
    }

    file.close();
    return true;
}

Course CourseManager::findByCode(const QString& code) const
{
    for (const Course& course : courses)
    {
        if (course.getCode().compare(
                code,
                Qt::CaseInsensitive) == 0)
        {
            return course;
        }
    }

    return Course();
}

std::vector<Course> CourseManager::findByName(
    const QString& keyword) const
{
    std::vector<Course> results;

    for (const Course& course : courses)
    {
        if (course.getName().contains(
                keyword,
                Qt::CaseInsensitive))
        {
            results.push_back(course);
        }
    }

    return results;
}

QString CourseManager::formatCourse(const Course& course) const
{
    return QString(
               "Course Code : %1\n"
               "Course Name : %2\n"
               "Credits : %3")
        .arg(course.getCode())
        .arg(course.getName())
        .arg(course.getCredits());
}

QString CourseManager::findAnswer(const QString& query) const
{
    // Priority 1: an explicit course code anywhere in the sentence always
    // wins — no need for fuzzy matching when the user was that specific.
    QString code = CourseCodeUtils::extractCourseCode(query);

    if(!code.isEmpty())
    {
        Course course = findByCode(code);

        if(!course.getCode().isEmpty())
        {
            return formatCourse(course);
        }
    }

    // Priority 2: no code (or an unrecognized one) — search course names
    // with the shared four-tier matcher. This is what makes a full name,
    // a partial name, or a slightly reworded name all work the same way.
    QVector<TextMatcher::Entry> entries;
    entries.reserve(static_cast<int>(courses.size()));

    for(int i = 0; i < static_cast<int>(courses.size()); ++i)
    {
        entries.append(
            TextMatcher::Entry{courses[static_cast<size_t>(i)].getName(), i});
    }

    TextMatcher::Result result = TextMatcher::findBestMatch(query, entries);

    if(result.matched())
    {
        return formatCourse(courses[static_cast<size_t>(result.index)]);
    }

    // Priority 3: WebCrawler, if one was provided.
    if(webCrawler)
    {
        QString crawled = webCrawler->search(query);

        if(!crawled.trimmed().isEmpty())
        {
            return crawled;
        }
    }

    return "Course not found.";
}