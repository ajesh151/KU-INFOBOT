#include "ChatBot.h"
#include "../crawler/WebCrawler.h"
#include <QDebug>
ChatBot::ChatBot(
    CourseManager* courseManager,
    RoutineManager* routineManager,
    FaqManager* faqManager,
    AdmissionManager* admissionManager,
    WebCrawler* webCrawler)
    : responseGenerator(courseManager,routineManager,faqManager,admissionManager),
    webCrawler(webCrawler)
{

}

QString ChatBot::getResponse(const QString& userInput)
{
    // Step 1: Typo correction
    QString processedInput = typoCorrector.correct(userInput);
    qDebug() << "AFTER TYPO CORRECTION:" << processedInput;

    processedInput = synonymManager.normalizeSentence(processedInput);
    qDebug() << "AFTER SYNONYM MAPPING:" << processedInput;

    Intent intent = intentRecognizer.recognizeIntent(processedInput);

    // Genuinely unrecognized intent — no manager to route to at all, so
    // this is the one case ChatBot handles the WebCrawler fallback itself.
    if(intent == Intent::UNKNOWN)
    {
        if(webCrawler)
        {
            QString crawled = webCrawler->search(processedInput);

            if(!crawled.trimmed().isEmpty())
            {
                return crawled;
            }
        }

        return "Sorry, I couldn't find information regarding your query.";
    }

    return responseGenerator.generateResponse(intent, processedInput);
}