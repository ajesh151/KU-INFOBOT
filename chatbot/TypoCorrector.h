#ifndef TYPOCORRECTOR_H
#define TYPOCORRECTOR_H

#include <QString>
#include <QHash>
#include <QSet>
#include <QRegularExpression>

// Corrects minor spelling mistakes in user input while preserving
// punctuation, capitalization, course codes, and known abbreviations.
//
// Correction strategy (in priority order):
//   1. Protected word check   -> never touch abbreviations / course codes
//   2. Exact dictionary match -> highest confidence, always preferred
//   3. Levenshtein fallback   -> only when no exact match exists, and only
//                                accepted when the edit distance is small
//                                relative to the word's length
class TypoCorrector
{
public:
    TypoCorrector();

    // Corrects a full sentence, preserving punctuation and capitalization.
    QString correct(const QString &sentence) const;

private:
    bool loadDictionary(const QString &filePath);

    // Returns true if `word` must never be altered (abbreviation or course code).
    bool isProtectedWord(const QString &word) const;

    // Returns true if `word` consists entirely of digits.
    bool isNumericToken(const QString &word) const;

    // Maximum Levenshtein distance allowed for a fuzzy match, scaled by length,
    // so short words require near-exact matches.
    int maxAllowedDistance(int wordLength) const;

    QString correctWord(const QString &word) const;

    int levenshteinDistance(const QString &a, const QString &b) const;

    QHash<QString, QString> dictionary;

    // Abbreviations that must never be "corrected" away, e.g. CE, CS, BIT.
    QSet<QString> protectedAbbreviations;

    // Matches course codes such as COSC101, CS101, BIT204A.
    static const QRegularExpression courseCodePattern;
};

#endif // TYPOCORRECTOR_H