#include "CrawlCache.h"

#include <QRegularExpression>
#include <QStringList>
#include <QFile>
#include <QTextStream>

namespace
{
// Weights for CrawlCache::scorePage. A keyword in the title is a much
// stronger relevance signal than one buried somewhere in the body
// text, so it's weighted proportionally higher.
const int TITLE_MATCH_WEIGHT = 10;
const int CONTENT_MATCH_WEIGHT = 2;

// Persistence record markers. title/url/content are always single-line
// by the time WebCrawler hands them over (already whitespace-
// simplified during extraction), so a fixed three-line-per-page record
// is safe and simple to parse back without needing an escaping scheme.
const QString URL_PREFIX = "URL:";
const QString TITLE_PREFIX = "TITLE:";
const QString CONTENT_PREFIX = "CONTENT:";
}

CrawlCache::CrawlCache()
{
}

void CrawlCache::addPage(const CachedPage &page)
{
    pages.append(page);
}

void CrawlCache::clear()
{
    pages.clear();
}

int CrawlCache::size() const
{
    return pages.size();
}

int CrawlCache::scorePage(const CachedPage &page, const QStringList &keywords) const
{
    QString lowerTitle = page.title.toLower();
    QString lowerContent = page.content.toLower();

    int score = 0;

    for(const QString &keyword : keywords)
    {
        QString key = keyword.trimmed().toLower();

        if(key.isEmpty())
        {
            continue;
        }

        if(lowerTitle.contains(key))
        {
            score += TITLE_MATCH_WEIGHT;
        }

        if(lowerContent.contains(key))
        {
            score += CONTENT_MATCH_WEIGHT;
        }
    }

    return score;
}

CachedPage CrawlCache::search(const QString &query) const
{
    if(pages.isEmpty())
    {
        return CachedPage();
    }

    QStringList keywords = query.toLower().split(
        QRegularExpression("\\s+"),
        Qt::SkipEmptyParts);

    CachedPage bestPage;
    int bestScore = -1;

    for(const CachedPage &page : pages)
    {
        int score = scorePage(page, keywords);

        if(score > bestScore)
        {
            bestScore = score;
            bestPage = page;
        }
    }

    return bestPage;
}

bool CrawlCache::saveToFile(const QString &filename) const
{
    QFile file(filename);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream out(&file);

    for(const CachedPage &page : pages)
    {
        out << URL_PREFIX << page.url << "\n";
        out << TITLE_PREFIX << page.title << "\n";
        out << CONTENT_PREFIX << page.content << "\n";
    }

    file.close();
    return true;
}

bool CrawlCache::loadFromFile(const QString &filename)
{
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    pages.clear();

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString urlLine = in.readLine();

        if(urlLine.isEmpty())
        {
            continue;
        }

        if(!urlLine.startsWith(URL_PREFIX))
        {
            // Unexpected format — stop rather than misinterpreting
            // arbitrary lines as page data.
            break;
        }

        QString titleLine = in.readLine();
        QString contentLine = in.readLine();

        CachedPage page;
        page.url = urlLine.mid(URL_PREFIX.length());
        page.title = titleLine.startsWith(TITLE_PREFIX)
                         ? titleLine.mid(TITLE_PREFIX.length())
                         : QString();
        page.content = contentLine.startsWith(CONTENT_PREFIX)
                           ? contentLine.mid(CONTENT_PREFIX.length())
                           : QString();

        pages.append(page);
    }

    file.close();
    return true;
}