#include "ProgramCodeUtils.h"

#include <QRegularExpression>
#include <QStringList>
#include <QVector>
#include <QPair>

QString ProgramCodeUtils::extractProgramCode(const QString& text)
{
    // Pass 1: short codes, exactly as they appear in routines.txt. Tried
    // first so a query typed with the raw abbreviation (or one that never
    // went through SynonymMapper at all) still works as before.
    static const QStringList knownCodes =
        {
            "BIT","BCA","BIM","BBA","CS","CE","EEE","BE","BPH","BSC","ME","CIVIL"
        };

    QString upperText = text.toUpper();

    for(const QString& code : knownCodes)
    {
        QRegularExpression pattern(
            "\\b" + QRegularExpression::escape(code) + "\\b");

        if(pattern.match(upperText).hasMatch())
        {
            return code;
        }
    }

    // Pass 2: full program names. This is what a NORMALIZED query actually
    // contains once SynonymMapper has expanded an abbreviation — e.g. "ce"
    // becomes "computer engineering" before RoutineManager ever sees it.
    // Keep this list in sync with the program phrase canonicals in
    // data/synonyms.txt (see the header comment for why it's duplicated).
    static const QVector<QPair<QString, QString>> fullNameToCode =
        {
            {"computer engineering", "CE"},
            {"computer science", "CS"},
            {"electrical engineering", "EEE"},
            {"bachelor of information technology", "BIT"},
            {"business administration", "BBA"},
            {"mechanical engineering", "ME"},
            {"civil engineering", "CIVIL"}
        };

    QString lowerText = text.toLower();

    for(const auto& entry : fullNameToCode)
    {
        if(lowerText.contains(entry.first))
        {
            return entry.second;
        }
    }

    return QString();
}