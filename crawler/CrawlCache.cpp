#include "CrawlCache.h"
#include <QStringList>
#include <QRegularExpression>

CrawlCache::CrawlCache()
{
}

void CrawlCache::addPage(const CachedPage &page)
{
    pages.push_back(page);
}

void CrawlCache::clear()
{
    pages.clear();
}

int CrawlCache::size() const
{
    return pages.size();
}

QVector<CachedPage> CrawlCache::getPages() const
{
    return pages;
}

int CrawlCache::scorePage(const CachedPage &page,
                          const QStringList &keywords) const
{
    int score = 0;

    QString title = page.title.toLower();
    QString content = page.content.toLower();

    for(const QString &keyword : keywords)
    {
        QString key = keyword.trimmed().toLower();

        if(key.isEmpty())
            continue;

        // Strong match if keyword appears in title
        if(title.contains(key))
            score += 10;

        // Weaker match if keyword appears in page content
        if(content.contains(key))
            score += 2;
    }

    return score;
}

CachedPage CrawlCache::search(const QString &query) const
{
    CachedPage bestPage;

    if(pages.isEmpty())
        return bestPage;

    QStringList keywords =
            query.toLower().split(
                QRegularExpression("\\s+"),
                Qt::SkipEmptyParts);

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