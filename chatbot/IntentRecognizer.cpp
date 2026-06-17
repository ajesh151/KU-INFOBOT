#include "IntentRecognizer.h"

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

    // Greetings

    if(containsAny(text,
                    {
                        "hello",
                        "hi",
                        "hey",
                        "namaste",
                        "good morning",
                        "good afternoon",
                        "good evening"
                    }))
    {
        return Intent::GREETING;
    }

    // Goodbye

    if(containsAny(text,
                    {
                        "bye",
                        "goodbye",
                        "see you",
                        "exit",
                        "quit"
                    }))
    {
        return Intent::GOODBYE;
    }

    // Routine Queries

    if(containsAny(text,
                    {
                        "routine",
                        "schedule",
                        "class timing",
                        "class",
                        "section",
                        "semester",
                        "year",
                        "venue",
                        "room"
                    }))
    {
        return Intent::ROUTINE_QUERY;
    }

    // Course Queries

    if(containsAny(text,
                    {
                        "course",
                        "subject",
                        "credit",
                        "credits",
                        "comp",
                        "eeeg",
                        "math",
                        "what is",
                        "course code"
                    }))
    {
        return Intent::COURSE_INFO;
    }

    // Admission Queries

    if(containsAny(text,
                    {
                        "admission",
                        "eligibility",
                        "fee",
                        "fees",
                        "kucat",
                        "entrance",
                        "apply",
                        "application",
                        "scholarship",
                        "merit"
                    }))
    {
        return Intent::ADMISSION_QUERY;
    }

    // FAQ Queries

    if(containsAny(text,
                    {
                        "library",
                        "hostel",
                        "office",
                        "location",
                        "university",
                        "ku",
                        "campus",
                        "contact"
                    }))
    {
        return Intent::FAQ_QUERY;
    }

    return Intent::UNKNOWN;
}