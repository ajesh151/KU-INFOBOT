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

    // Persists all cached pages to a plain-text file, so a crawl survives
    // between runs — crawling the live site is slow and network-dependent,
    // and there's no reason to redo it on every fresh launch if a previous
    // crawl already exists.
    bool saveToFile(const QString &filename) const;

    // Loads pages previously written by saveToFile(). Clears any pages
    // currently in memory first. Returns false if the file doesn't exist
    // or can't be read (this is expected on a completely fresh setup,
    // before any crawl has ever run — not an error condition callers need
    // to treat specially).
    bool loadFromFile(const QString &filename);

private:
    QVector<CachedPage> pages;
    int scorePage(const CachedPage &page,
                  const QStringList &keywords) const;
};
#endif // CRAWLCACHE_H