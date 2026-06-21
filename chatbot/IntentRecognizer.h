#ifndef INTENTRECOGNIZER_H
#define INTENTRECOGNIZER_H
#include <QString>
#include <QStringList>

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

/*
class IntentRecognizer
{
public:
    IntentRecognizer();

    // Intent Recognition
    QString recognizeIntent(const QString &input) const;

    // Entity Extraction
    QString extractProgram(const QString &input) const;
    int extractYear(const QString &input) const;
    int extractSemester(const QString &input) const;
    QString extractCourseCode(const QString &input) const;

    // Validation
    bool hasProgram(const QString &input) const;
    bool hasYear(const QString &input) const;
    bool hasSemester(const QString &input) const;
    bool hasCourseCode(const QString &input) const;

    // Query Type
    bool isProgramQuery(const QString &input) const;
    bool isProgramSemesterQuery(const QString &input) const;
    bool isCourseCodeQuery(const QString &input) const;
};

#endif // INTENTRECOGNIZER_H


*/