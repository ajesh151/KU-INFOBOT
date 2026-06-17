#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H

#include <vector>
#include <QString>
#include "../models/Course.h"

class CourseManager
{
private:
    std::vector<Course> courses;

public:
    CourseManager();

    bool loadCourses(const QString& filename);

    std::vector<Course> getAllCourses() const;

    Course findByCode(const QString& code) const;

    std::vector<Course> findByName(const QString& keyword) const;

    std::vector<Course> findByPrefix(const QString& prefix) const;
};

#endif