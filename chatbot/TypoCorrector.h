#ifndef TYPOCORRECTOR_H
#define TYPOCORRECTOR_H

#include <QString>
#include <QHash>

class TypoCorrector
{
    public:

        TypoCorrector();
        bool loadDictionary(const QString &filePath);
        QString correct(const QString &sentence) const;

    private:

        QHash<QString, QString> dictionary;
        QString correctWord(const QString &word) const;
        int levenshteinDistance(const QString &a,const QString &b) const;
};

#endif // TYPOCORRECTOR_H