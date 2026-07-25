#ifndef CURRICULUMMANAGER_H
#define CURRICULUMMANAGER_H

#include <vector>
#include "Curriculum.h"

class CurriculumManager
{
public:

    bool loadCurriculum(const QString& filename);

    QString findCurriculum(const QString& query) const;

private:

    std::vector<Curriculum> curriculum;

    QString extractProgram(const QString& text) const;

    int extractYear(const QString& text) const;

    int extractSemester(const QString& text) const;
};

#endif