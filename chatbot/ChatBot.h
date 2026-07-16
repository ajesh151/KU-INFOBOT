#ifndef CHATBOT_H
#define CHATBOT_H

#include <QString>

#include "IntentRecognizer.h"
#include "ResponseGenerator.h"
#include "TypoCorrector.h"
#include "SynonymMapper.h"
#include "../crawler/WebCrawler.h"

class ChatBot
{
private:
    IntentRecognizer intentRecognizer;
    ResponseGenerator responseGenerator;
    TypoCorrector typoCorrector;
    SynonymMapper synonymManager;
    WebCrawler webCrawler;
    

public:
    ChatBot(
        CourseManager* courseManager,
        RoutineManager* routineManager,
        FaqManager* faqManager,
        AdmissionManager* admissionManager);

    QString getResponse(const QString& userInput);
};

#endif // CHATBOT_H