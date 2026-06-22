#include "FaqManager.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>

FaqManager::FaqManager()
{
}

bool FaqManager::loadFaqs(const QString& filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    faqs.clear();

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
        {
            continue;
        }

        QStringList parts = line.split('|');

        if (parts.size() != 2)
        {
            continue;
        }

        Faq faq(
            parts[0].trimmed(), // Question
            parts[1].trimmed()  // Answer
        );

        faqs.push_back(faq);
    }

    file.close();
    return true;
}

std::vector<Faq> FaqManager::getAllFaqs() const
{
    return faqs;
}

QString FaqManager::findAnswer(const QString& question) const
{
    QString input = question.toLower();

    int bestScore = 0;
    QString bestAnswer;

    for(const Faq& faq : faqs)
    {
        QString storedQuestion = faq.getQuestion().toLower();

        QStringList words =
            storedQuestion.split(
                QRegularExpression("\\W+"),
                Qt::SkipEmptyParts);

        int score = 0;

        for(const QString& word : words)
        {
            if(word.length() < 3)
            {
                continue;
            }

            if(input.contains(word))
            {
                score++;
            }
        }

        if(score > bestScore)
        {
            bestScore = score;
            bestAnswer = faq.getAnswer();
        }
    }

    if(bestScore >= 2)
    {
        return bestAnswer;
    }

    return "Sorry, I couldn't find an answer for that question.";
}

std::vector<Faq> FaqManager::searchFaqs(
    const QString& keyword) const
{
    std::vector<Faq> results;

    for (const Faq& faq : faqs)
    {
        if (faq.getQuestion().contains(
                keyword,
                Qt::CaseInsensitive) ||
            faq.getAnswer().contains(
                keyword,
                Qt::CaseInsensitive))
        {
            results.push_back(faq);
        }
    }

    return results;
}