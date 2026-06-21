#include "IntentRecognizer.h"
#include <QRegularExpression>
#include <QStringList>
#include<QString>
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

    // Greeting
    if(text.contains("hello") ||
        text.contains("hi") ||
        text.contains("hey"))
    {
        return Intent::GREETING;
    }

    // Goodbye
    if(text.contains("bye") ||
        text.contains("goodbye"))
    {
        return Intent::GOODBYE;
    }

    // Course code detection
    QRegularExpression coursePattern(
        "[A-Z]{4,5}\\d{3}",
        QRegularExpression::CaseInsensitiveOption);

    if(coursePattern.match(input).hasMatch())
    {
        return Intent::COURSE_INFO;
    }

    // Routine
    if(text.contains("routine") ||
        text.contains("schedule") ||
        text.contains("class"))
    {
        return Intent::ROUTINE_QUERY;
    }

    // Admission
    if(text.contains("admission") ||
        text.contains("apply") ||
        text.contains("entrance"))
    {
        return Intent::ADMISSION_QUERY;
    }

    // FAQ
    return Intent::FAQ_QUERY;
}


