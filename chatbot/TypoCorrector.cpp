#include "TypoCorrector.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QVector>
#include <climits>

const QRegularExpression TypoCorrector::courseCodePattern(
    "^[A-Za-z]{2,5}\\d{2,4}[A-Za-z]*$");

TypoCorrector::TypoCorrector()
{
    // Known abbreviations that must be preserved exactly as typed.
    // Extend this list as new program/subject abbreviations are introduced.
    protectedAbbreviations =
        {
            "ce", "cs", "bit", "bca", "bim", "bba", "eee",
            "ai", "be", "bph", "bsc", "me","ku",
        "kucat",
        "cbt",
        "gpa",
        "cgpa",
        "phd",
        "mba",
        "mbe",
        "mppm",
        "bbis",
        "bbm",
        "llb",
        "ib",
        "ctevt",
        "neb",
        "lmtc"
        };

    loadDictionary("data/typo_dictionary.txt");
}

bool TypoCorrector::loadDictionary(const QString &filePath)
{
    dictionary.clear();

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        if (line.startsWith("#"))
            continue;

        QStringList parts = line.split("=");

        if (parts.size() != 2)
            continue;

        QString wrong = parts[0].trimmed().toLower();
        QString correct = parts[1].trimmed();

        dictionary.insert(wrong, correct);
    }

    file.close();

    return true;
}

bool TypoCorrector::isProtectedWord(const QString &word) const
{
    QString lower = word.toLower();

    if (protectedAbbreviations.contains(lower))
        return true;

    if (courseCodePattern.match(word).hasMatch())
        return true;

    return false;
}

int TypoCorrector::maxAllowedDistance(int wordLength) const
{
    if (wordLength <= 3)
        return 1;

    if (wordLength <= 6)
        return 2;

    return 3;
}

int TypoCorrector::levenshteinDistance(const QString &a,
                                       const QString &b) const
{
    int n = a.length();
    int m = b.length();

    QVector<QVector<int>> dp(n + 1,
                             QVector<int>(m + 1));

    for (int i = 0; i <= n; i++)
        dp[i][0] = i;

    for (int j = 0; j <= m; j++)
        dp[0][j] = j;

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            int cost =
                (a[i - 1] == b[j - 1]) ? 0 : 1;

            int deletion =
                dp[i - 1][j] + 1;

            int insertion =
                dp[i][j - 1] + 1;

            int substitution =
                dp[i - 1][j - 1] + cost;

            dp[i][j] =
                qMin(deletion,
                     qMin(insertion,
                          substitution));
        }
    }

    return dp[n][m];
}

QString TypoCorrector::correctWord(const QString &word) const
{
    if (word.isEmpty())
        return word;

    // Never touch abbreviations or course codes, regardless of dictionary
    // or fuzzy-match results.
    if (isProtectedWord(word))
        return word;

    QString originalWord = word;
    QString lowerWord = word.toLower();

    // Priority 1: exact dictionary match (highest confidence).
    if (dictionary.contains(lowerWord))
    {
        QString corrected = dictionary.value(lowerWord);

        if (originalWord[0].isUpper())
        {
            corrected[0] = corrected[0].toUpper();
        }

        return corrected;
    }

    // Priority 2: fuzzy match, only reached when no exact match exists.
    // The accepted distance scales with word length so short words
    // require a near-exact match before being altered.
    QString bestMatch = originalWord;
    int bestDistance = INT_MAX;

    for (auto it = dictionary.constBegin();
         it != dictionary.constEnd();
         ++it)
    {
        int distance = levenshteinDistance(lowerWord, it.key());

        if (distance < bestDistance)
        {
            bestDistance = distance;
            bestMatch = it.value();
        }
    }

    int allowedDistance = maxAllowedDistance(lowerWord.length());

    if (bestDistance <= allowedDistance)
    {
        if (originalWord[0].isUpper())
        {
            bestMatch[0] = bestMatch[0].toUpper();
        }

        return bestMatch;
    }

    // Not confident enough to change the word — leave it as typed.
    return originalWord;
}

QString TypoCorrector::correct(const QString &sentence) const
{
    QString result;
    QString currentWord;

    for (int i = 0; i < sentence.length(); ++i)
    {
        QChar ch = sentence[i];

        if (ch.isLetterOrNumber())
        {
            currentWord += ch;
        }
        else
        {
            if (!currentWord.isEmpty())
            {
                result += correctWord(currentWord);
                currentWord.clear();
            }

            result += ch;
        }
    }

    if (!currentWord.isEmpty())
    {
        result += correctWord(currentWord);
    }

    return result;
}