#include "FaqManager.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

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
    for (const Faq& faq : faqs)
    {
        if (faq.getQuestion().compare(
                question,
                Qt::CaseInsensitive) == 0)
        {
            return faq.getAnswer();
        }
    }

    return "Sorry, I could not find an answer to that question.";
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