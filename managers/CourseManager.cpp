#include "CourseManager.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

CourseManager::CourseManager()
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

std::vector<Course> CourseManager::getAllCourses() const
{
    return courses;
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

std::vector<Course> CourseManager::findByPrefix(
    const QString& prefix) const
{
    std::vector<Course> results;

    for (const Course& course : courses)
    {
        if (course.getCode().startsWith(
                prefix,
                Qt::CaseInsensitive))
        {
            results.push_back(course);
        }
    }

    return results;
}