#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H

#include <QString>
#include <vector>

#include "../models/Course.h"

class WebCrawler; // forward declaration only — full include lives in the .cpp

// Owns the course database. findAnswer() handles both explicit course codes
// ("Tell me about COSC101") and course names, including partial ones
// ("Information about Programming Fundamentals"), via the shared
// TextMatcher. Falls back to WebCrawler when there's no local match.
class CourseManager
{
public:
    // webCrawler may be nullptr (e.g. in unit tests) — findAnswer simply
    // skips the crawler fallback in that case.
    explicit CourseManager(WebCrawler* webCrawler = nullptr);

    bool loadCourses(const QString& filename);

    std::vector<Course> getAllCourses() const;

    Course findByCode(const QString& code) const;

    std::vector<Course> findByName(const QString& keyword) const;

    std::vector<Course> findByPrefix(const QString& prefix) const;

    // If `query` contains an explicit course code anywhere in it, looks
    // that up directly. Otherwise runs TextMatcher's four-tier search over
    // course names (this is what makes partial names like "programming
    // fundamentals" or "fundamentals" work). Falls back to WebCrawler, and
    // only then reports "Course not found."
    QString findAnswer(const QString& query) const;

private:
    QString formatCourse(const Course& course) const;

    std::vector<Course> courses;
    WebCrawler* webCrawler;
};

#endif // COURSEMANAGER_H