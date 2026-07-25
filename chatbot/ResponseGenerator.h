#ifndef RESPONSEGENERATOR_H
#define RESPONSEGENERATOR_H

#include <QString>

#include "IntentRecognizer.h"

class CourseManager;
class RoutineManager;
class FaqManager;
class AdmissionManager;
class CurriculumManager;

// Pure router: given an intent and the normalized query, forwards to the
// correct manager and returns its response. ResponseGenerator must never
// search, match, or know anything about a manager's internal data — that
// logic lives in the managers (via the shared TextMatcher) and, as a last
// resort inside each manager, WebCrawler.
class ResponseGenerator
{
public:
    ResponseGenerator(
        CourseManager* courseManager,
        RoutineManager* routineManager,
        FaqManager* faqManager,
        AdmissionManager* admissionManager,
        CurriculumManager* curriculumManager);

    QString generateResponse(
        Intent intent,
        const QString& userInput) const;

private:
    CourseManager* courseManager;
    RoutineManager* routineManager;
    FaqManager* faqManager;
    AdmissionManager* admissionManager;
    CurriculumManager* curriculumManager;
};

#endif // RESPONSEGENERATOR_H