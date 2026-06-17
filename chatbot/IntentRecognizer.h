#ifndef INTENTRECOGNIZER_H
#define INTENTRECOGNIZER_H

#include <QString>

enum class Intent
{
    COURSE_INFO,
    ROUTINE_QUERY,
    FAQ_QUERY,
    ADMISSION_QUERY,
    GREETING,
    GOODBYE,
    UNKNOWN
};

class IntentRecognizer
{
public:
    IntentRecognizer();

    Intent recognizeIntent(const QString& input) const;

private:
    bool containsAny(
        const QString& text,
        const QStringList& keywords) const;
};

#endif // INTENTRECOGNIZER_H