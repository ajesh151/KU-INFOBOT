#include "ResponseGenerator.h"
#include "../managers/CourseManager.h"
#include "../managers/RoutineManager.h"
#include "../managers/FAQManager.h"
#include "../managers/AdmissionManager.h"

ResponseGenerator::ResponseGenerator(
    CourseManager* courseManager,
    RoutineManager* routineManager,
    FaqManager* faqManager,
    AdmissionManager* admissionManager)
    :
    courseManager(courseManager),
    routineManager(routineManager),
    faqManager(faqManager),
    admissionManager(admissionManager)
{
}

QString ResponseGenerator::generateResponse(
    Intent intent,
    const QString& userInput) const
{
    switch(intent)
    {
    case Intent::GREETING:
        return "Hello! How can I help you today?";

    case Intent::GOODBYE:
        return "Goodbye! Have a great day.";

    case Intent::COURSE_INFO:
        return courseManager->findAnswer(userInput);

    case Intent::ROUTINE_QUERY:
        return routineManager->findAnswer(userInput);

    case Intent::FAQ_QUERY:
        return faqManager->findAnswer(userInput);

    case Intent::ADMISSION_QUERY:
        return admissionManager->findAnswer(userInput);

    case Intent::UNKNOWN:
    default:
        return "Sorry, I could not understand your question.";
    }
}