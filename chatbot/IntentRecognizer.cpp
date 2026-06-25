#include "IntentRecognizer.h"
#include <QRegularExpression>
#include <QStringList>
#include <QString>
IntentRecognizer::IntentRecognizer()
{}

bool IntentRecognizer::containsAny(const QString& text, const QStringList& keywords) const
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

    static const QStringList greetingKeywords = {"hello", "hi", "hey","namaste","namaskar"};
    static const QStringList goodbyeKeywords = {"bye", "goodbye","see you"};
    static const QStringList routineKeywords = {"routine", "schedule", "class","timetable","semester","year", "section",
                                                 "sunday","monday","tuesday","wednesday","thursday","friday","saturday"};
    static const QStringList admissionKeywords = {"admission", "apply", "entrance"};
    static const QStringList faqKeywords ={"what","how","why","when","where","facility","hostel","library"
                                            ,"wifi","transport","club","event","fee","fees","scholarship"};
    // Greeting
    if(containsAny(text, greetingKeywords))
    {
        return Intent::GREETING;
    }

    // Goodbye
    if(containsAny(text, goodbyeKeywords))
    {
        return Intent::GOODBYE;
    }

    if(containsAny(text, routineKeywords))
    {
        return Intent::ROUTINE_QUERY;
    }

    static const QRegularExpression coursePattern(
        "[A-Z]{3,5}\\d{3}",
        QRegularExpression::CaseInsensitiveOption);

    if(coursePattern.match(input).hasMatch())
    {
        return Intent::COURSE_INFO;
    }

    // Admission
    if(containsAny(text, admissionKeywords))
    {
        return Intent::ADMISSION_QUERY;
    }
    if(containsAny(text, faqKeywords))
    {
        return Intent::FAQ_QUERY;
    }

    // FAQ
    return Intent::FAQ_QUERY;
}