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
    const QString& normalizedInput) const
{
    // Defensive lowercasing only — normalizedInput should already be
    // lowercase coming out of SynonymMapper. This is not a substitute for
    // normalization; raw, un-normalized text should never reach this method.
    QString text = normalizedInput.toLower();


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
        coursePattern.match(normalizedInput).hasMatch();


    // Note: "schedule" and "timetable" normalize to "routine" upstream in
    // SynonymMapper, and "email"/"phone" normalize to "contact" in the FAQ
    // list below. They're kept here too as a defensive fallback in case this
    // method is ever exercised directly (e.g. in tests) without going
    // through the full pipeline — harmless redundancy, not the primary path.
    static const QStringList routineKeywords =
        {
            "routine","schedule","timetable","semester","year", "section",
            "sunday","monday","tuesday","wednesday","thursday","friday","saturday",
            "today","tomorrow","class timing","class time","next class","which class"
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
        "admit","enrollment","enrolment","registration",
        "program change","cutoff","cut off","admission",
        "eligibility",
        "pcb eligibility",
        "gce a level",
        "ib diploma",
        "ctevt diploma",
        "application",
        "online registration",
        "application process",
        "program change",
        "enrollment confirmation",
        "architecture admission",
        "geomatics scholarship",
        "tie breaking",
        "entrance exam",
        "kucat", "kucbt",
        "computer based test",
        "test format",
        "difficulty level",
        "adaptive scoring",
        "negative marking",
        "pass mark",
        "minimum score",
        "documents",
        "international students",
        "scholarship",
        "fee",
        "deadline",
        "merit list",
        };

    if(containsAny(text, admissionKeywords))
    {
        return Intent::ADMISSION_QUERY;
    }

    static const QStringList curriculumKeywords =
        {
            "curriculum","course catalogue","course catalog","catalogue","catalog",
            "course structure","course list","subjects list","all subjects",
            "syllabus structure","full course list","which subjects"
        };

    if(containsAny(text, curriculumKeywords))
    {
        return Intent::CURRICULUM_QUERY;
    }


    static const QStringList faqKeywords =
        {
            "ku",
            "kathmandu university",
            "library",
            "hostel",
            "website",
            "internet",
            "canteen",
            "bus",
            "transport",
            "club",
            "clubs",
            "event",
            "events",
            "facility",
            "facilities",
            "contact",
            "location",
            "office",
            "email",
            "phone",
            "does ku"
        };

    if(containsAny(text, faqKeywords))
    {
        return Intent::FAQ_QUERY;
    }

    // If no intent matches, return UNKNOWN so the web crawler
    // or fallback mechanism can handle the query.
    return Intent::UNKNOWN;
}