#ifndef CHATBOT_H
#define CHATBOT_H

#include <QString>

#include "TypoCorrector.h"
#include "SynonymMapper.h"
#include "IntentRecognizer.h"
#include "ResponseGenerator.h"

class CourseManager;
class RoutineManager;
class FaqManager;
class AdmissionManager;
class CurriculumManager;
class WebCrawler;

class ChatBot
{
public:
    // webCrawler is shared with AdmissionManager/FaqManager/CourseManager
    // so there's a single crawl cache instead of one per component. It is
    // only used here as the final fallback for genuinely UNKNOWN intent —
    // each manager owns its own "no local match" fallback internally.
    ChatBot(
        CourseManager* courseManager,
        RoutineManager* routineManager,
        FaqManager* faqManager,
        AdmissionManager* admissionManager,
        CurriculumManager* curriculumManager,
        WebCrawler* webCrawler);

    QString getResponse(const QString& userInput);

private:
    TypoCorrector typoCorrector;
    SynonymMapper synonymManager;
    IntentRecognizer intentRecognizer;
    ResponseGenerator responseGenerator;
    WebCrawler* webCrawler;
};

#endif // CHATBOT_H