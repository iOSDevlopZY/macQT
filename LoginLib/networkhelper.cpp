#include "networkhelper.h"
#include <QNetworkRequest>
#include <QEventLoop>
#include <QDebug>

NetworkHelper::NetworkHelper(QObject *parent) : QObject(parent)
{
    netManager = new QNetworkAccessManager(this);
}

NetworkHelper::~NetworkHelper()
{
    delete netManager;
}

/**
 * @brief GET数据
 * @param 地址
 * @return
 */
QByteArray NetworkHelper::get(QString url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *Reply = netManager->get(request);
    connect (Reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(netError(QNetworkReply::NetworkError)));
    QEventLoop eventLoop;
    connect(netManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    disconnect(netManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QByteArray data = Reply->readAll();
    Reply->deleteLater();
    return data;
}

void NetworkHelper::netError(QNetworkReply::NetworkError err)
{
    Q_UNUSED(err);
    qWarning()<<QString::fromLocal8Bit ("ERROR:网络错误");
    emit netErr();
}


QByteArray NetworkHelper::post(QString urlStr,QString Parm)
{
    QUrl url(urlStr);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postArray;
    postArray.append(Parm);

    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.length());

    QNetworkReply *Reply = netManager->post(request, postArray);
    connect (Reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(netError(QNetworkReply::NetworkError)));
    QEventLoop eventLoop;
    connect(netManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    disconnect(netManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QByteArray data = Reply->readAll();
    Reply->deleteLater();
    return data;
}

QByteArray NetworkHelper::getNetPicture(QString url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));

    QNetworkReply *Reply = netManager->get(request);
    connect (Reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(netError(QNetworkReply::NetworkError)));
    QEventLoop eventLoop;
    connect(netManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    disconnect(netManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QByteArray data = Reply->readAll();
    Reply->deleteLater();
    return data;
}

/**
 * @brief POST请求，带参数
 * @param urlStr
 * @param parm
 * @return
 */
QByteArray NetworkHelper::postRequestWithParam(QString urlStr, QString parm)
{
    QUrl url(urlStr);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postArray;
    postArray.append(parm);

    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.length());

    QNetworkReply *Reply = netManager->post(request, postArray);
    connect (Reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(netError(QNetworkReply::NetworkError)));
    QEventLoop eventLoop;
    connect(netManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    disconnect(netManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QByteArray data = Reply->readAll();
    Reply->deleteLater();

    return data;
}
