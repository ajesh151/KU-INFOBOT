#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H

#include <QString>
#include <QStringList>
#include <QSet>

#include "CrawlCache.h"

class WebCrawler
{
public:
    WebCrawler();

    // Crawl the website starting from the given URL
    void crawl(const QString &startUrl);

    // Search for the closest matching webpage
    QString search(const QString &query);

private:
    CrawlCache cache;
    QSet<QString> visitedUrls;

    // Downloads HTML of a webpage
    QString downloadPage(const QString &url);

    // Extract page title
    QString extractTitle(const QString &html);

    // Remove HTML tags to get plain text
    QString extractContent(const QString &html);

    // Extract internal KU links
    QStringList extractLinks(const QString &html,
                             const QString &baseUrl);

    // Crawl a single page (recursive helper)
    void crawlPage(const QString &url);

    // Check whether a URL belongs to KU
    bool isKuUrl(const QString &url) const;
};

#endif // WEBCRAWLER_H