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
};

#endif // TYPOCORRECTOR_H