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

    if(containsAny(text, faqKeywords))
    {
        return Intent::FAQ_QUERY;
    }


    return Intent::FAQ_QUERY;
}