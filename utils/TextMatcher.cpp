#include "TextMatcher.h"

#include <QRegularExpression>
#include <QSet>
#include <algorithm>
#include <climits>

namespace
{
// Small, deliberately conservative stopword list. Only affects Tier 4
// (keyword scoring) — Tiers 1-3 use full keys, unfiltered, since exact
// and phrase/word matches should never be weakened by stopword removal.
const QSet<QString> stopWords =
    {
        "a","an","the","is","are","was","were","of","for","to","in","on","at",
        "and","or","what","how","do","does","did","i","you","me","my",
        "please","tell","about","can","could","would","will","it","this","that"
};

// Length of the longest common (contiguous) substring between two
// already-lowercased strings. Used only as a Tier 4 tie-breaker — a
// rough "closeness" signal when two entries score the same.
int longestCommonSubstringLength(const QString& a, const QString& b)
{
    int n = a.length();
    int m = b.length();

    QVector<QVector<int>> dp(n + 1, QVector<int>(m + 1, 0));
    int best = 0;

    for(int i = 1; i <= n; ++i)
    {
        for(int j = 1; j <= m; ++j)
        {
            if(a[i - 1] == b[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                best = std::max(best, dp[i][j]);
            }
        }
    }

    return best;
}
}

QStringList TextMatcher::tokenize(const QString& text)
{
    static const QRegularExpression wordPattern("[A-Za-z0-9]+");

    QStringList words;
    auto it = wordPattern.globalMatch(text.toLower());

    while(it.hasNext())
    {
        words.append(it.next().captured(0));
    }

    return words;
}

bool TextMatcher::containsWholePhrase(
    const QString& lowerHaystack,
    const QString& lowerPhrase)
{
    if(lowerPhrase.isEmpty())
        return false;

    int pos = 0;

    while((pos = lowerHaystack.indexOf(lowerPhrase, pos)) != -1)
    {
        bool leftOk =
            (pos == 0) ||
            !lowerHaystack.at(pos - 1).isLetterOrNumber();

        int endPos = pos + lowerPhrase.length();

        bool rightOk =
            (endPos >= lowerHaystack.length()) ||
            !lowerHaystack.at(endPos).isLetterOrNumber();

        if(leftOk && rightOk)
            return true;

        pos += 1;
    }

    return false;
}

int TextMatcher::findExactMatch(
    const QString& query,
    const QVector<Entry>& entries)
{
    QString normalizedQuery = query.trimmed().toLower();

    for(const Entry& entry : entries)
    {
        if(entry.key.trimmed().toLower() == normalizedQuery)
        {
            return entry.index;
        }
    }

    return -1;
}

int TextMatcher::findPhraseMatch(
    const QString& query,
    const QVector<Entry>& entries)
{
    QString lowerQuery = query.toLower();

    int bestIndex = -1;
    int bestWordCount = -1;
    int bestLength = -1;

    for(const Entry& entry : entries)
    {
        QString lowerKey = entry.key.trimmed().toLower();

        if(lowerKey.isEmpty())
            continue;

        if(!containsWholePhrase(lowerQuery, lowerKey))
            continue;

        int wordCount = tokenize(lowerKey).size();
        int length = lowerKey.length();

        bool better =
            (wordCount > bestWordCount) ||
            (wordCount == bestWordCount && length > bestLength);

        if(better)
        {
            bestIndex = entry.index;
            bestWordCount = wordCount;
            bestLength = length;
        }
    }

    return bestIndex;
}

int TextMatcher::findWordMatch(
    const QString& query,
    const QVector<Entry>& entries)
{
    QStringList queryWordList = tokenize(query);
    QSet<QString> queryWords(queryWordList.begin(), queryWordList.end());

    int bestIndex = -1;
    int bestWordCount = -1;
    int bestLength = INT_MAX;

    for(const Entry& entry : entries)
    {
        QStringList keyWords = tokenize(entry.key);

        if(keyWords.isEmpty())
            continue;

        bool allPresent = true;

        for(const QString& word : keyWords)
        {
            if(!queryWords.contains(word))
            {
                allPresent = false;
                break;
            }
        }

        if(!allPresent)
            continue;

        int wordCount = keyWords.size();
        int length = entry.key.length();

        bool better =
            (wordCount > bestWordCount) ||
            (wordCount == bestWordCount && length < bestLength);

        if(better)
        {
            bestIndex = entry.index;
            bestWordCount = wordCount;
            bestLength = length;
        }
    }

    return bestIndex;
}

int TextMatcher::findKeywordMatch(
    const QString& query,
    const QVector<Entry>& entries)
{
    QString lowerQuery = query.toLower();
    QStringList queryWordList = tokenize(query);
    QSet<QString> queryWords(queryWordList.begin(), queryWordList.end());

    int bestIndex = -1;
    int bestScore = 0;
    int bestLcs = -1;
    int bestLength = INT_MAX;

    for(const Entry& entry : entries)
    {
        QStringList keyWords = tokenize(entry.key);
        int score = 0;

        for(const QString& word : keyWords)
        {
            if(word.length() < 3 || stopWords.contains(word))
                continue;

            if(queryWords.contains(word))
            {
                ++score;
            }
        }

        if(score <= 0)
            continue;

        int lcs = longestCommonSubstringLength(lowerQuery, entry.key.toLower());
        int length = entry.key.length();

        bool better = false;

        if(score > bestScore)
        {
            better = true;
        }
        else if(score == bestScore)
        {
            if(lcs > bestLcs)
            {
                better = true;
            }
            else if(lcs == bestLcs && length < bestLength)
            {
                better = true;
            }
            // Equal on score, lcs, and length: keep the earlier occurrence
            // (first-come tie-break) by simply not overwriting.
        }

        if(better)
        {
            bestIndex = entry.index;
            bestScore = score;
            bestLcs = lcs;
            bestLength = length;
        }
    }

    return bestIndex;
}

TextMatcher::Result TextMatcher::findBestMatch(
    const QString& query,
    const QVector<Entry>& entries)
{
    if(query.trimmed().isEmpty() || entries.isEmpty())
        return Result();

    int index = findExactMatch(query, entries);

    if(index != -1)
        return Result{index};

    index = findPhraseMatch(query, entries);

    if(index != -1)
        return Result{index};

    index = findWordMatch(query, entries);

    if(index != -1)
        return Result{index};

    index = findKeywordMatch(query, entries);

    return Result{index};
}