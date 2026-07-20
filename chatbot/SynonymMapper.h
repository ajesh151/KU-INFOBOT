#ifndef SYNONYMMAPPER_H
#define SYNONYMMAPPER_H

#include <QString>
#include <QHash>
#include <QVector>

// Normalizes user input by collapsing synonyms, abbreviations, and
// alternate phrasings into a single canonical form, so downstream
// components (IntentRecognizer, ResponseGenerator, Managers) only
// ever have to deal with one "spelling" of each concept.
//
// Storage format (data/synonyms.txt):
//   canonical = variant1, variant2, ...
//
//   - canonical may be a single word or a phrase (e.g. "computer engineering").
//   - Each variant is classified automatically:
//       * contains a space  -> phrase synonym  (matched before word synonyms)
//       * no space          -> word synonym    (also used for abbreviations,
//                                                e.g. "computer engineering = ce")
//
// Normalization order (both required by the refactoring plan):
//   1. Phrase synonyms, longest variant first, matched on tokenized words
//      so a phrase can never match inside a larger word.
//   2. Word synonyms, one token at a time, single lookup only (no chained
//      resolution) — punctuation attached to a token is preserved.
class SynonymMapper
{
public:
    SynonymMapper();

    // Normalizes a full sentence: phrase pass, then word pass.
    QString normalizeSentence(const QString& sentence) const;

    // Normalizes a single word (single-hop lookup; kept public for testability).
    QString normalizeWord(const QString& word) const;

private:
    bool loadFromFile(const QString& filename);

    // Single-hop lookup for an already-lowercased word. Returns an empty
    // string if there is no mapping (deliberately NOT chained/recursive —
    // one lookup is enough, and chaining risks unintended cascades if the
    // data file changes).
    QString lookupWord(const QString& lowerWord) const;

    // variant -> canonical (single words, incl. abbreviations expanding to phrases)
    QHash<QString, QString> wordSynonyms;

    // "word1 word2 ..." (space-joined, normalized) -> canonical
    QHash<QString, QString> phraseSynonyms;

    // first word of a phrase variant -> list of phrase keys (into
    // phraseSynonyms) starting with that word, sorted longest-first.
    // This turns phrase lookup from "scan every phrase against the whole
    // sentence" into "look up candidates for the word we're already at".
    QHash<QString, QVector<QString>> phrasesByFirstWord;
};

#endif // SYNONYMMAPPER_H