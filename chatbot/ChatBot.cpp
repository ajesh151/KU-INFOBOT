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
    // Step 1: Typo correction
    QString processedInput = typoCorrector.correct(userInput);

    // Step 2: Synonym normalization (phrase synonyms, then word synonyms)
    processedInput = synonymManager.normalizeSentence(processedInput);

    // Step 3: Intent recognition
    Intent intent = intentRecognizer.recognizeIntent(processedInput);

    if(intent == Intent::UNKNOWN)
    {
        return webCrawler.search(processedInput);
    }

    return responseGenerator.generateResponse(intent, processedInput);
}