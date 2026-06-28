#include "IntentRecognizer.h"

#include <QRegularExpression>
#include <QString>
#include <QStringList>

IntentRecognizer::IntentRecognizer()
{
}

bool IntentRecognizer::containsAny(
    const QString& text,
    const QStringList& keywords) const
{
    for(const QString& keyword : keywords)
    {
        if(text.contains(keyword, Qt::CaseInsensitive))
        {
            return true;
        }
    }

    return false;
}

Intent IntentRecognizer::recognizeIntent(
    const QString& input) const
{
    QString text = input.toLower();


    static const QStringList greetingKeywords =
    {
        "hello","hi","hey","heyy","namaste","namaskar",
        "good morning","good afternoon","good evening"
    };

    if(containsAny(text, greetingKeywords))
    {
        return Intent::GREETING;
    }


    static const QStringList goodbyeKeywords =
    {
        "bye","goodbye","see you","take care","later","bye bye"
    };

    if(containsAny(text, goodbyeKeywords))
    {
        return Intent::GOODBYE;
    }

    
    static const QRegularExpression coursePattern(
        "[A-Z]{3,5}\\d{3}",
        QRegularExpression::CaseInsensitiveOption);

    bool hasCourseCode =
            coursePattern.match(input).hasMatch();


    static const QStringList routineKeywords =
    {
        "routine","schedule","timetable","semester","year","section",
        "sunday","monday","tuesday","wednesday","thursday","friday","saturday",
       "today","tomorrow","class timing","class time","next class","which class","when is"
    };

    if(containsAny(text,routineKeywords))
    {
        return Intent::ROUTINE_QUERY;
    }

    
    if(hasCourseCode)
    {
        if(text.contains("when") ||
           text.contains("routine") ||
           text.contains("schedule") ||
           text.contains("time"))
        {
            return Intent::ROUTINE_QUERY;
        }

        return Intent::COURSE_INFO;
    }

    static const QStringList admissionKeywords =
    {
        "admission","admissions","apply","application","eligibility","eligible",
        "required documents","documents","entrance","entrance exam","entrance examination",
        "fee","fees","tuition","payment","scholarship","financial aid","deadline",
        "admit","enrollment","enrolment","registration"
    };

    if(containsAny(text, admissionKeywords))
    {
        return Intent::ADMISSION_QUERY;
    }


    static const QStringList faqKeywords =
    {
        "what","how","why","where","library","hostel","wifi",
        "internet","canteen","bus","transport","club","clubs","event",
        "events","facility","facilities","contact","location","office",
        "email","phone"
    };

case Intent::FAQ_QUERY:
{
    QString text = userInput.toLower();

    if(text.contains("your name") ||
       text.contains("who are you") ||
       text == "name" ||
       text == "name?")
    {
        return "Hi! 😊 I'm KU InfoBot, your virtual assistant for Kathmandu University. I'm here to help you with information about courses, routines, admissions, fees, scholarships, and more.";
    }

    if(text.contains("what can you do"))
    {
        return "I can answer questions related to:\n\n"
               "📘 Courses\n"
               "📅 Class Routines\n"
               "🎓 Admissions\n"
               "📝 Entrance Exams\n"
               "💰 Fee Structure\n"
               "🏆 Scholarships\n"
               "🏫 Facilities\n"
               "❓ General FAQs";
    }

    if(text.contains("thank"))
    {
        return "You're welcome! 😊 If you have any more questions, feel free to ask.";
    }

    if(text.contains("how are you"))
    {
        return "I'm doing great! 😊 Thank you for asking. How can I assist you today?";
    }
    if(text.contains("what is ku infobot") ||
   text.contains("what is kuinfo bot") ||
   text.contains("tell me about ku infobot") ||
   text.contains("explain ku infobot") ||
   text.contains("what is this chatbot"))
    {
        return "🤖 KU InfoBot is a rule-based virtual assistant developed to provide quick and accurate information about Kathmandu University. "
           "It can answer questions related to courses, class routines, admissions, entrance examinations, fee structures, scholarships, facilities, and other frequently asked questions. "
           "Its goal is to help students, applicants, parents, and visitors access university information easily without manually searching through documents or websites.";
    }

    return faqManager->findAnswer(userInput);
}