#include "WebCrawler.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QUrl>
#include <QDebug>

WebCrawler::WebCrawler()
{
}

QString WebCrawler::downloadPage(const QString &url)
{
    QNetworkAccessManager manager;

    QNetworkRequest request;
    request.setUrl(QUrl(url));

    request.setHeader(
        QNetworkRequest::UserAgentHeader,
        "KU-INFOBOT/1.0");

    QEventLoop loop;

    QNetworkReply *reply = manager.get(request);

    QObject::connect(reply,
                     &QNetworkReply::finished,
                     &loop,
                     &QEventLoop::quit);

    loop.exec();

    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Download failed:" << url;
        qDebug() << reply->errorString();

        reply->deleteLater();
        return "";
    }

    QString html = QString::fromUtf8(reply->readAll());

    reply->deleteLater();

    return html;
}

bool WebCrawler::isKuUrl(const QString &url) const
{
    QString lower = url.toLower();

    // Main KU website
    if(lower.contains("ku.edu.np"))
        return true;

    // School of Law
    if(lower.contains("kusol.edu.np"))
        return true;

    // School of Engineering
    if(lower.contains("soe.ku.edu.np"))
        return true;

    // School of Medical Sciences
    if(lower.contains("kusms.edu.np"))
        return true;

    // Kathmandu University Online
    if(lower.contains("konline.ku.edu.np"))
        return true;

    return false;
}

QString WebCrawler::extractTitle(const QString &html)
{
    QRegularExpression titleRegex(
        "<title[^>]*>(.*?)</title>",
        QRegularExpression::CaseInsensitiveOption |
        QRegularExpression::DotMatchesEverythingOption);

    QRegularExpressionMatch match = titleRegex.match(html);

    if(match.hasMatch())
    {
        QString title = match.captured(1);

        title.remove(QRegularExpression("<[^>]*>"));
        title = title.simplified();

        return title;
    }

    return "";
}

QString WebCrawler::extractContent(const QString &html)
{
    QString content = html;

    // Remove script blocks
    content.remove(
        QRegularExpression(
            "<script[^>]*>.*?</script>",
            QRegularExpression::CaseInsensitiveOption |
            QRegularExpression::DotMatchesEverythingOption));

    // Remove style blocks
    content.remove(
        QRegularExpression(
            "<style[^>]*>.*?</style>",
            QRegularExpression::CaseInsensitiveOption |
            QRegularExpression::DotMatchesEverythingOption));

    // Remove all HTML tags
    content.remove(
        QRegularExpression("<[^>]+>"));

    // Replace common HTML entities
    content.replace("&nbsp;", " ");
    content.replace("&amp;", "&");
    content.replace("&quot;", "\"");
    content.replace("&lt;", "<");
    content.replace("&gt;", ">");

    // Remove extra whitespace
    content = content.simplified();

    return content;
}

QStringList WebCrawler::extractLinks(const QString &html,
                                     const QString &baseUrl)
{
    QStringList links;

    QRegularExpression linkRegex(
        "<a[^>]+href\\s*=\\s*['\"]([^'\"]+)['\"]",
        QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatchIterator iterator =
            linkRegex.globalMatch(html);

    while(iterator.hasNext())
    {
        QRegularExpressionMatch match = iterator.next();

        QString link = match.captured(1).trimmed();

        if(link.isEmpty())
            continue;

        // Ignore anchors
        if(link.startsWith("#"))
            continue;

        // Ignore email links
        if(link.startsWith("mailto:"))
            continue;

        // Ignore javascript links
        if(link.startsWith("javascript:"))
            continue;

        QUrl absoluteUrl =
                QUrl(baseUrl).resolved(QUrl(link));

        QString finalUrl =
                absoluteUrl.toString(QUrl::RemoveFragment);

        if(isKuUrl(finalUrl))
        {
            if(!links.contains(finalUrl))
                links.append(finalUrl);
        }
    }

    return links;
}

void WebCrawler::crawlPage(const QString &url)
{
    // Already visited
    if(visitedUrls.contains(url))
        return;

    visitedUrls.insert(url);

    qDebug() << "Crawling:" << url;

    QString html = downloadPage(url);

    if(html.isEmpty())
        return;

    CachedPage page;

    page.title = extractTitle(html);
    page.url = url;
    page.content = extractContent(html);

    cache.addPage(page);

    // Discover more KU links
    QStringList links = extractLinks(html, url);

    for(const QString &link : links)
    {
        if(!visitedUrls.contains(link))
        {
            crawlPage(link);
        }
    }
}

void WebCrawler::crawl(const QString &startUrl)
{
    cache.clear();
    visitedUrls.clear();

    crawlPage(startUrl);

    qDebug() << "Finished crawling.";
    qDebug() << "Pages indexed:" << cache.size();
}

QString WebCrawler::search(const QString &query)
{
    // Crawl only once
    if(cache.size() == 0)
    {
        crawl("https://ku.edu.np/");
    }

    CachedPage result = cache.search(query);

    if(result.url.isEmpty())
    {
        return "No matching KU webpage found.";
    }

    QString response;

    response += "I couldn't find a predefined answer.\n\n";
    response += "The closest matching Kathmandu University webpage is:\n\n";
    response += result.title;
    response += "\n";
    response += result.url;

    return response;
}