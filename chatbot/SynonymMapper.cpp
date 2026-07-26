#include "SynonymMapper.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>
#include <algorithm>

namespace
{
// A single token from the input: either a run of letters/digits (a word)
// or a run of anything else (a separator — spaces, punctuation, hyphens,
// underscores, slashes, etc). Treating ANY non-alphanumeric run as a
// separator means "computer-engineering" and "computer_engineering" are
// recognized as the two words "computer" and "engineering", the same as
// "computer engineering" — not just literal spaces.
struct Token
{
    QString text;
    bool isWord = false;
};

QVector<Token> tokenize(const QString& text)
{
    static const QRegularExpression pattern(
        "[A-Za-z0-9]+|[^A-Za-z0-9]+");

    QVector<Token> tokens;

    auto it = pattern.globalMatch(text);

    while(it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        QString piece = match.captured(0);

        Token token;
        token.text = piece;
        token.isWord = piece.at(0).isLetterOrNumber();

        tokens.append(token);
    }

    return tokens;
}

// Splits a phrase key ("word1 word2 ...") back into its individual words.
QStringList splitPhraseKey(const QString& phraseKey)
{
    return phraseKey.split(' ', Qt::SkipEmptyParts);
}
}

SynonymMapper::SynonymMapper()
{
    loadFromFile("data/synonyms.txt");
}

bool SynonymMapper::loadFromFile(const QString& filename)
{
    wordSynonyms.clear();
    phraseSynonyms.clear();
    phrasesByFirstWord.clear();

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    static const QRegularExpression wordSplitter("[^A-Za-z0-9]+");

    while(!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if(line.isEmpty() || line.startsWith('#'))
            continue;

        int equalPos = line.indexOf('=');

        if(equalPos == -1)
        {
            continue;
        }

        QString canonical = line.left(equalPos).trimmed().toLower();

        if(canonical.isEmpty())
        {
            continue;
        }

        QStringList variants =
            line.mid(equalPos + 1).split(',', Qt::SkipEmptyParts);

        if(variants.isEmpty())
        {
            continue;
        }

        for(QString variant : variants)
        {
            variant = variant.trimmed().toLower();

            if(variant.isEmpty())
                continue;

            if(variant.contains(' '))
            {
                // Phrase variant — normalize its internal spacing so lookups
                // are consistent regardless of how it was typed in the file.
                QString phraseKey =
                    splitPhraseKey(variant).join(' ');

                if(phraseSynonyms.contains(phraseKey) &&
                    phraseSynonyms.value(phraseKey) != canonical)
                {
                    qWarning() << "SynonymMapper: variant phrase" << phraseKey
                               << "already maps to"
                               << phraseSynonyms.value(phraseKey)
                               << "- ignoring duplicate mapping to" << canonical;
                    continue;
                }

                phraseSynonyms[phraseKey] = canonical;

                QString firstWord = splitPhraseKey(phraseKey).first();
                phrasesByFirstWord[firstWord].append(phraseKey);
            }
            else
            {
                if(wordSynonyms.contains(variant) &&
                    wordSynonyms.value(variant) != canonical)
                {
                    qWarning() << "SynonymMapper: variant" << variant
                               << "already maps to" << wordSynonyms.value(variant)
                               << "- ignoring duplicate mapping to" << canonical;
                    continue;
                }

                wordSynonyms[variant] = canonical;
            }
        }

        // Explicit self-mapping for single-word canonicals, so behavior
        // doesn't depend on the fallback path — a canonical always resolves
        // to itself as a real entry, not just "nothing matched, keep as-is".
        if(!canonical.contains(' ') && !wordSynonyms.contains(canonical))
        {
            wordSynonyms[canonical] = canonical;
        }
    }

    file.close();

    // Sort each first-word group longest-phrase-first (by word count, then
    // character length), so replacement order is deterministic and a longer
    // phrase is always preferred over a shorter one that overlaps it.
    for(auto it = phrasesByFirstWord.begin();
         it != phrasesByFirstWord.end();
         ++it)
    {
        std::sort(it.value().begin(),
                  it.value().end(),
                  [](const QString& a, const QString& b)
                  {
                      int wordsA = splitPhraseKey(a).size();
                      int wordsB = splitPhraseKey(b).size();

                      if(wordsA != wordsB)
                          return wordsA > wordsB;

                      return a.length() > b.length();
                  });
    }

    return true;
}

QString SynonymMapper::lookupWord(const QString& lowerWord) const
{
    auto it = wordSynonyms.constFind(lowerWord);

    if(it != wordSynonyms.constEnd())
        return it.value();

    return QString();
}

QString SynonymMapper::normalizeWord(const QString& word) const
{
    QString lower = word.toLower();
    QString mapped = lookupWord(lower);

    return mapped.isEmpty() ? lower : mapped;
}

QString SynonymMapper::normalizeSentence(const QString& sentence) const
{
    QVector<Token> tokens = tokenize(sentence.toLower());

    // Word-token indices into `tokens`, in order, paired with their text —
    // used for phrase matching without re-scanning separators every time.
    QVector<int> wordTokenIndices;
    QStringList words;

    for(int i = 0; i < tokens.size(); ++i)
    {
        if(tokens[i].isWord)
        {
            wordTokenIndices.append(i);
            words.append(tokens[i].text);
        }
    }

    QVector<bool> consumed(tokens.size(), false);
    QVector<QString> replacement(tokens.size());

    // --- Phrase pass: must run before the word pass ---
    int wi = 0;

    while(wi < words.size())
    {
        const QVector<QString>& candidates =
            phrasesByFirstWord.value(words[wi]);

        bool matched = false;

        for(const QString& phraseKey : candidates)
        {
            QStringList candidateWords = splitPhraseKey(phraseKey);
            int len = candidateWords.size();

            if(wi + len > words.size())
                continue;

            bool allMatch = true;

            for(int k = 0; k < len; ++k)
            {
                if(words[wi + k] != candidateWords[k])
                {
                    allMatch = false;
                    break;
                }
            }

            if(!allMatch)
                continue;

            QString canonical = phraseSynonyms.value(phraseKey);

            int firstTokenIdx = wordTokenIndices[wi];
            int lastTokenIdx = wordTokenIndices[wi + len - 1];

            // The canonical text takes the place of the first matched word;
            // everything from just after it through the last matched word
            // (including any separators between them) is dropped, since
            // it's now folded into the canonical replacement.
            replacement[firstTokenIdx] = canonical;

            for(int t = firstTokenIdx + 1; t <= lastTokenIdx; ++t)
            {
                consumed[t] = true;
            }

            wi += len;
            matched = true;
            break;
        }

        if(!matched)
        {
            ++wi;
        }
    }

    // --- Word pass: only touches word-tokens the phrase pass didn't use ---
    for(int idx : wordTokenIndices)
    {
        if(consumed[idx] || !replacement[idx].isEmpty())
            continue;

        QString mapped = lookupWord(tokens[idx].text);

        if(!mapped.isEmpty())
        {
            replacement[idx] = mapped;
        }
    }

    // --- Reassemble, preserving every separator that wasn't consumed ---
    QString result;

    for(int i = 0; i < tokens.size(); ++i)
    {
        if(consumed[i])
            continue;

        result += replacement[i].isEmpty() ? tokens[i].text : replacement[i];
    }

    return result;
}