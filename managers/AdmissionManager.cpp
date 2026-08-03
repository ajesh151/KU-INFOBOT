#include "AdmissionManager.h"
#include "../utils/TextMatcher.h"
#include "../crawler/WebCrawler.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

AdmissionManager::AdmissionManager(WebCrawler* webCrawler)
    : webCrawler(webCrawler)
{
}

bool AdmissionManager::loadData(const QString& filename)
{
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if(line.isEmpty())
            continue;

        if(line.startsWith("#"))
            continue;

        QStringList parts = line.split('|');

        if(parts.size() != 2)
            continue;

        // Same "\n" -> real newline convention as FaqManager, for any
        // admission/fee/entrance answer that needs multi-line formatting.
        QString answer = parts[1].trimmed();
        answer.replace("\\n", "\n");

        admissions.push_back(
            Admission(
                parts[0].trimmed(),
                answer));
    }

    file.close();

    return true;
}

bool AdmissionManager::loadAllSources(const QStringList& filenames)
{
    bool allSucceeded = true;

    for(const QString& filename : filenames)
    {
        if(!loadData(filename))
        {
            allSucceeded = false;
        }
    }

    return allSucceeded;
}

std::vector<Admission> AdmissionManager::getAllData() const
{
    return admissions;
}

QString AdmissionManager::findAnswer(const QString& question) const
{
    QVector<TextMatcher::Entry> entries;
    entries.reserve(static_cast<int>(admissions.size()));

    for(int i = 0; i < static_cast<int>(admissions.size()); ++i)
    {
        entries.append(
            TextMatcher::Entry{admissions[static_cast<size_t>(i)].getQuestion(), i});
    }

    TextMatcher::Result result = TextMatcher::findBestMatch(question, entries);

    if(result.matched())
    {
        return admissions[static_cast<size_t>(result.index)].getAnswer();
    }

    if(webCrawler)
    {
        QString crawled = webCrawler->search(question);

        if(!crawled.trimmed().isEmpty())
        {
            return crawled;
        }
    }

    return "Sorry, I couldn't find any admission-related information.";
}

std::vector<Admission> AdmissionManager::searchData(
    const QString& keyword) const
{
    std::vector<Admission> results;

    QString search = keyword.toLower();

    for(const Admission &admission : admissions)
    {
        QString key =
            admission.getQuestion().toLower();

        if(key.contains(search))
        {
            results.push_back(admission);
        }
    }

    return results;
}