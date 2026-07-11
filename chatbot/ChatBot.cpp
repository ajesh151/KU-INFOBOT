#include "ChatBot.h"

ChatBot::ChatBot(
    CourseManager* courseManager,
    RoutineManager* routineManager,
    FaqManager* faqManager,
    AdmissionManager* admissionManager)
    : responseGenerator(courseManager,routineManager,faqManager,admissionManager)
{
    
}

QString ChatBot::getResponse(const QString& userInput)
{
    // Step 1: Correct typos
    QString processedInput =typoCorrector.correct(userInput);

    // Step 2: Recognize intent
    Intent intent =intentRecognizer.recognizeIntent(processedInput);

    // Step 3: Unknown → Web crawler fallback
    if(intent == Intent::UNKNOWN)
    {
        return webCrawler.search(processedInput);
    }

    // Step 4: Known intent
    return responseGenerator.generateResponse(intent,processedInput);
}