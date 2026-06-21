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
    static const QStringList goodbyeKeywords = {"bye", "goodbye"};
    static const QStringList routineKeywords = {"routine", "schedule", "class"};
    static const QStringList admissionKeywords = {"admission", "apply", "entrance"};

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

    // FAQ
    return Intent::FAQ_QUERY;
}