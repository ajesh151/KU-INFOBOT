#ifndef TEXTMATCHER_H
#define TEXTMATCHER_H

#include <QString>
#include <QStringList>
#include <QVector>

class TextMatcher
{
public:
    struct Entry
    {
        QString key;
        int index;
    };

    struct Result
    {
        int index = -1;

        bool found() const
        {
            return index != -1;
        }
    };

    static Result findBestMatch(
        const QString& query,
        const QVector<Entry>& entries);

private:
    static QStringList tokenize(
        const QString& text);

    static bool containsWholePhrase(
        const QString& lowerHaystack,
        const QString& lowerPhrase);

    static int findExactMatch(
        const QString& query,
        const QVector<Entry>& entries);

    static int findPhraseMatch(
        const QString& query,
        const QVector<Entry>& entries);

    static int findWordMatch(
        const QString& query,
        const QVector<Entry>& entries);

    static int findKeywordMatch(
        const QString& query,
        const QVector<Entry>& entries);
};

#endif // TEXTMATCHER_H