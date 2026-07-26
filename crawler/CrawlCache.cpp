#include "CrawlCache.h"
#include <QStringList>
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>

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

bool CrawlCache::saveToFile(const QString &filename) const
{
    QFile file(filename);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    // title/url/content are all already whitespace-simplified by
    // WebCrawler's extraction (no embedded newlines), so a fixed
    // three-line-per-page record is safe and simple to parse back.
    for(const CachedPage &page : pages)
    {
        out << "URL:" << page.url << "\n";
        out << "TITLE:" << page.title << "\n";
        out << "CONTENT:" << page.content << "\n";
    }

    file.close();
    return true;
}

bool CrawlCache::loadFromFile(const QString &filename)
{
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    pages.clear();

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString urlLine = in.readLine();

        if(urlLine.isEmpty())
            continue;

        if(!urlLine.startsWith("URL:"))
        {
            // Unexpected format — stop rather than misinterpreting
            // arbitrary lines as page data.
            break;
        }

        QString titleLine = in.readLine();
        QString contentLine = in.readLine();

        CachedPage page;
        page.url = urlLine.mid(4);
        page.title = titleLine.startsWith("TITLE:") ? titleLine.mid(6) : QString();
        page.content = contentLine.startsWith("CONTENT:") ? contentLine.mid(8) : QString();

        pages.append(page);
    }

    file.close();
    return true;
}