#include "AdmissionManager.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>

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
        {
            continue;
        }

        QStringList parts = line.split('|');

        if(parts.size() != 2)
        {
            continue;
        }

        Admission admission(
            parts[0].trimmed(),
            parts[1].trimmed());

        admissions.push_back(admission);
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
    QString input = question.toLower();

    int bestScore = 0;
    QString bestAnswer;

    for(const Admission& admission : admissions)
    {
        QString storedQuestion =
                admission.getQuestion().toLower();

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
            bestAnswer = admission.getAnswer();
        }
    }

    if(bestScore >= 2)
    {
        return bestAnswer;
    }

    return "Sorry, I couldn't find any admission-related information.";
}

std::vector<Admission> AdmissionManager::searchData(
        const QString& keyword) const
{
    std::vector<Admission> results;

    for(const Admission& admission : admissions)
    {
        if(admission.getQuestion().contains(
                    keyword,
                    Qt::CaseInsensitive)
            ||
            admission.getAnswer().contains(
                    keyword,
                    Qt::CaseInsensitive))
        {
            results.push_back(admission);
        }
    }

    return results;
}