#include "AdmissionManager.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>
#include <QSet>

AdmissionManager::AdmissionManager()
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

        admissions.push_back(
            Admission(
                parts[0].trimmed(),
                parts[1].trimmed()));
    }

    file.close();

    return true;
}

std::vector<Admission> AdmissionManager::getAllData() const
{
    return admissions;
}

QString AdmissionManager::findAnswer(const QString& question) const
{
    QString input = question.toLower().trimmed();

    QStringList inputWords =
        input.split(QRegularExpression("\\W+"),
                    Qt::SkipEmptyParts);

    int bestScore = -1;
    QString bestAnswer;

    for(const Admission &admission : admissions)
    {
        QString key =
            admission.getQuestion().toLower().trimmed();

        QStringList keyWords =
            key.split(QRegularExpression("\\W+"),
                      Qt::SkipEmptyParts);

        int score = 0;

        //--------------------------------------------------
        // Priority 1 : Exact key
        //--------------------------------------------------
        if(input == key)
        {
            return admission.getAnswer();
        }

        //--------------------------------------------------
        // Priority 2 : Phrase match
        //--------------------------------------------------
        if(input.contains(key))
        {
            score += 10000;
        }

        //--------------------------------------------------
        // Priority 3 : All keywords present
        //--------------------------------------------------
        bool allPresent = true;

        for(const QString &word : keyWords)
        {
            if(!inputWords.contains(word))
            {
                allPresent = false;
                break;
            }
        }

        if(allPresent)
        {
            score += 5000;
        }

        //--------------------------------------------------
        // Priority 4 : Keyword overlap
        //--------------------------------------------------
        int matchedWords = 0;

        for(const QString &word : keyWords)
        {
            if(inputWords.contains(word))
            {
                matchedWords++;
            }
        }

        score += matchedWords * 100;

        //--------------------------------------------------
        // Bonus for longer, more specific keys
        //--------------------------------------------------
        score += keyWords.size();

        //--------------------------------------------------
        // Keep best
        //--------------------------------------------------
        if(score > bestScore)
        {
            bestScore = score;
            bestAnswer = admission.getAnswer();
        }
    }

    if(bestScore >= 100)
    {
        return bestAnswer;
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