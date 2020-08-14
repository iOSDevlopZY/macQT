#include "networkhelper.h"
#include <QEventLoop>

// 静态变量需要在类体的外面进行初始化
NetworkHelper *NetworkHelper::m_Instance = NULL;

NetworkHelper::NetworkHelper(QObject *parent) : QObject(parent)
{
    netManager = new QNetworkAccessManager(this);
}

NetworkHelper::~NetworkHelper()
{
    if(m_Instance)
    {
        delete m_Instance;
        m_Instance = NULL;
    }
    if(netManager)
    {
        netManager->deleteLater();
    }
}

/**
 * @brief 单实例方法
 * @return
 */
NetworkHelper *NetworkHelper::sharedInstance()
{
    if(m_Instance == NULL)
    {
        m_Instance = new NetworkHelper();
    }
    return m_Instance;
}

/**
 * @brief 上传文件
 * @param url
 * @param 文件流
 */
QByteArray NetworkHelper::uploadFile(QString urlStr, QByteArray fileByte)
{
    QUrl url(urlStr);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postArray;
    postArray.append(fileByte);

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

/**
 * @brief POST请求
 * @param url
 * @return
 */
QByteArray NetworkHelper::postRequest(QString urlStr)
{
    QUrl url(urlStr);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postArray;

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

/**
 * @brief 请求出现错误
 * @param 错误
 */
void NetworkHelper::netError(QNetworkReply::NetworkError err)
{
    Q_UNUSED(err);
    emit netError();
}
