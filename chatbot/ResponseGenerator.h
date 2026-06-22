#ifndef RESPONSEGENERATOR_H
#define RESPONSEGENERATOR_H

#include <QString>

#include "../managers/CourseManager.h"
#include "../managers/RoutineManager.h"
#include "../managers/FAQManager.h"
#include "../managers/AdmissionManager.h"
#include "IntentRecognizer.h"

class ResponseGenerator
{
private:
    CourseManager* courseManager;
    RoutineManager* routineManager;
   FaqManager* faqManager;
    //AdmissionManager* admissionManager;

public:
   ResponseGenerator(CourseManager* courseManager,RoutineManager* routineManager, FaqManager* faqManager);
        // AdmissionManager* admissionManager

    QString generateResponse(
        Intent intent,
        const QString& userInput) const;
};

#endif // RESPONSEGENERATOR_H