#include "ChatBot.h"
#include <iostream>
using namespace std;
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
    // Step 1
    QString processedInput = typoCorrector.correct(userInput);

    // Step 2
    std::string normalized =
        synonymManager.normalizeSentence(
            processedInput.toStdString());


    processedInput =
        QString::fromStdString(normalized);
 cout << normalized;
    // Step 3
    Intent intent =
        intentRecognizer.recognizeIntent(processedInput);

    if(intent == Intent::UNKNOWN)
    {
        return webCrawler.search(processedInput);
    }

    return responseGenerator.generateResponse(intent,
                                              processedInput);
}