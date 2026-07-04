#include "WebCrawler.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QUrl>

WebCrawler::WebCrawler()
{
}

QString WebCrawler::downloadPage(const QString &url)
{
    QNetworkAccessManager manager;

    QNetworkRequest request(QUrl(url));

    request.setHeader(
        QNetworkRequest::UserAgentHeader,
        "KU-INFOBOT WebCrawler");

    QEventLoop loop;

    QNetworkReply *reply = manager.get(request);

    QObject::connect(reply,
                     &QNetworkReply::finished,
                     &loop,
                     &QEventLoop::quit);

    loop.exec();

    if(reply->error() != QNetworkReply::NoError)
    {
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

    if(lower.contains("ku.edu.np"))
        return true;

    if(lower.contains("kusol.edu.np"))
        return true;

    if(lower.contains("soe.ku.edu.np"))
        return true;

    if(lower.contains("som.ku.edu.np"))
        return true;

    if(lower.contains("konline.ku.edu.np"))
        return true;

    return false;
}