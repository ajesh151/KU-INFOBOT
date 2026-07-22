#ifndef TEXTMATCHER_H
#define TEXTMATCHER_H

#include <QString>
#include <QStringList>
#include <QVector>

// Shared search utility used by every manager that searches free-text
// question/answer style data (AdmissionManager, FaqManager, CourseManager's
// name lookup, etc). RoutineManager does NOT use this: its data is
// structured (program/year/semester/section/day/course code), and should
// stay a field-based search rather than being forced through text matching.
//
// Every caller gets the exact same search priority:
//   1. Exact match     — query equals a stored key exactly (case-insensitive)
//   2. Phrase match    — the stored key appears as a whole phrase in the query
//   3. Word match      — every word of the stored key appears in the query
//   4. Keyword match   — highest overlap score wins (stopwords ignored)
//
// If all four tiers fail, the caller should fall back to WebCrawler itself —
// TextMatcher only ever reports "no local match" (index == -1); it knows
// nothing about WebCrawler, ResponseGenerator, or any manager's data model.
class TextMatcher
{
public:
    // One searchable entry. `key` is the stored question/name being matched
    // against; `index` is whatever the caller needs to look the full record
    // back up (e.g. an index into their own std::vector/QList).
    struct Entry
    {
        QString key;
        int index = -1;
    };

    struct Result
    {
        int index = -1;
        bool matched() const { return index != -1; }
    };

    // Runs all four tiers in order, stopping at the first one that
    // produces a match. Returns Result{-1} if nothing matched at any tier.
    static Result findBestMatch(
        const QString& query,
        const QVector<Entry>& entries);

private:
    static QStringList tokenize(const QString& text);

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