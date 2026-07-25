#include "FAQManager.h"
#include "../utils/TextMatcher.h"
#include "../crawler/WebCrawler.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

FaqManager::FaqManager(WebCrawler* webCrawler)
    : webCrawler(webCrawler)
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

        if (line.startsWith("#"))
        {
            continue;
        }

        QStringList parts = line.split('|');

        if (parts.size() != 2)
        {
            continue;
        }

        // Data files are line-based, so a literal multi-line answer can't
        // be stored directly. Authors write "\n" in the text file and it's
        // converted to a real newline here, e.g.:
        //   what can you do|Courses\nRoutines\nAdmissions
        QString answer = parts[1].trimmed();
        answer.replace("\\n", "\n");

        Faq faq(
            parts[0].trimmed(), // Question
            answer               // Answer
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
    QVector<TextMatcher::Entry> entries;
    entries.reserve(static_cast<int>(faqs.size()));

    for(int i = 0; i < static_cast<int>(faqs.size()); ++i)
    {
        entries.append(
            TextMatcher::Entry{faqs[static_cast<size_t>(i)].getQuestion(), i});
    }

    TextMatcher::Result result = TextMatcher::findBestMatch(question, entries);

    if(result.matched())
    {
        return faqs[static_cast<size_t>(result.index)].getAnswer();
    }

    if(webCrawler)
    {
        QString crawled = webCrawler->search(question);

        if(!crawled.trimmed().isEmpty())
        {
            return crawled;
        }
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