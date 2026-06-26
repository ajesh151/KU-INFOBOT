#ifndef CHATBOT_H
#define CHATBOT_H

#include <QString>

#include "IntentRecognizer.h"
#include "ResponseGenerator.h"
#include "TypoCorrector.h"
#include "SynonymMapper.h"

class ChatBot
{
private:
    IntentRecognizer intentRecognizer;
    ResponseGenerator responseGenerator;
    /*TypoCorrector typoCorrector;
    SynonymManager synonymManager;*/

public:
    ChatBot(
        CourseManager* courseManager,
        RoutineManager* routineManager,
        FaqManager* faqManager,
        AdmissionManager* admissionManager);

    QString getResponse(const QString& userInput);
};

#endif // CHATBOT_H