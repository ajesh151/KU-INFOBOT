#include "TypoCorrector.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>
#include <QVector>
#include <climits>

TypoCorrector::TypoCorrector()
{
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

    QString originalWord = word;
    QString lowerWord = word.toLower();

    // Exact dictionary match
    if (dictionary.contains(lowerWord))
    {
        QString corrected = dictionary.value(lowerWord);

        // Preserve capitalization
        if (originalWord[0].isUpper())
        {
            corrected[0] = corrected[0].toUpper();
        }

        return corrected;
    }

    // Fuzzy matching using Levenshtein distance
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

    // Only accept close matches
    if (bestDistance <= 2)
    {
        if (originalWord[0].isUpper())
        {
            bestMatch[0] = bestMatch[0].toUpper();
        }

        return bestMatch;
    }

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