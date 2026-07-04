#include "TypoCorrector.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>

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

        if (line.startsWith('#'))
            continue;

        QStringList parts = line.split('=');

        if (parts.size() != 2)
            continue;

        QString wrong = parts[0].trimmed().toLower();
        QString correct = parts[1].trimmed();

        dictionary.insert(wrong, correct);
    }

    file.close();
    return true;
}

QString TypoCorrector::correctWord(const QString &word) const
{
    QString lower = word.toLower();

    if (dictionary.contains(lower))
        return dictionary.value(lower);

    return word;
}

QString TypoCorrector::correct(const QString &sentence) const
{
    QString result = sentence;

    QRegularExpression re("\\b\\w+\\b");
    QRegularExpressionMatchIterator i = re.globalMatch(sentence);

    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();

        QString word = match.captured();
        QString corrected = correctWord(word);

        result.replace(match.capturedStart(),
                       word.length(),
                       corrected);
    }

    return result;
}