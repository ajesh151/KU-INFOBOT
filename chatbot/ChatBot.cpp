#include "ChatBot.h"
#include <QDebug>
ChatBot::ChatBot(
    CourseManager* courseManager,
    RoutineManager* routineManager,
    FaqManager* faqManager,
    AdmissionManager* admissionManager)
    : responseGenerator(
          courseManager,
          routineManager,
          faqManager,
          admissionManager)
{
}

QString ChatBot::getResponse(
    const QString& userInput)
{
    // Step 1: Correct typos

    // QString processedInput =
    //     typoCorrector.correct(userInput);

    // Step 2: Replace synonyms

    // processedInput =
    //     synonymManager.replaceSynonyms(
    //         processedInput);

    // Step 3: Detect intent
    QString processedInput = userInput;
    Intent intent = intentRecognizer.recognizeIntent(processedInput);

    // Step 4: Generate response

    return responseGenerator.generateResponse(
        intent,
        processedInput);
}