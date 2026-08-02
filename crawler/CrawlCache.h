#ifndef CRAWLCACHE_H
#define CRAWLCACHE_H

#include <QString>
#include <QVector>

// One crawled webpage: its title, URL, and plain-text content (HTML
// already stripped out by the time this is populated).
struct CachedPage
{
    QString title;
    QString url;
    QString content;
};

// Holds every page WebCrawler has indexed and finds the best match for a
// free-text query among them. Knows nothing about crawling itself — it's
// purely a store-and-search structure, kept separate from WebCrawler so
// "how pages are found" and "how a query is matched against what's been
// found" stay independent concerns.
class CrawlCache
{
public:
    CrawlCache();

    // Adds one page to the cache.
    void addPage(const CachedPage &page);

    // Removes every cached page.
    void clear();

    // Number of pages currently cached.
    int size() const;

    // Returns the single best-scoring page for `query`, or a default
    // (empty) CachedPage if the cache is empty. Scoring is a simple
    // keyword-overlap count — title matches count for more than content
    // matches, since a keyword appearing in the page's own title is a much
    // stronger signal of relevance than it appearing somewhere in the body
    // text.
    CachedPage search(const QString &query) const;

    // Persists all cached pages to a plain-text file, so a crawl survives
    // between runs. Crawling the live site is slow and network-dependent;
    // there's no reason to redo it on every fresh launch if a previous
    // crawl already exists on disk.
    bool saveToFile(const QString &filename) const;

    // Loads pages previously written by saveToFile(), replacing whatever
    // is currently in memory. Returns false if the file doesn't exist or
    // can't be read — expected and not an error on a completely fresh
    // setup, before any crawl has ever run.
    bool loadFromFile(const QString &filename);

private:
    int scorePage(const CachedPage &page, const QStringList &keywords) const;

    QVector<CachedPage> pages;
};

#endif // CRAWLCACHE_H