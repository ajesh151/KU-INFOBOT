#ifndef CRAWLCACHE_H
#define CRAWLCACHE_H

#include <QString>
#include <QVector>

/*
 * Stores information about one crawled webpage.
 */
struct CachedPage
{
    QString title;
    QString url;
    QString content;
};

class CrawlCache
{
public:
    CrawlCache();

    // Add a page to the cache
    void addPage(const CachedPage &page);

    // Remove all cached pages
    void clear();

    // Number of cached pages
    int size() const;

    // Return the best matching page for a query
    CachedPage search(const QString &query) const;

    // Get all cached pages
    QVector<CachedPage> getPages() const;

private:
    QVector<CachedPage> pages;

    int scorePage(const CachedPage &page,
                  const QStringList &keywords) const;
};

#endif // CRAWLCACHE_H