#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class NetworkHelper : public QObject
{
    Q_OBJECT
public:
    explicit NetworkHelper(QObject *parent = nullptr);
    ~NetworkHelper();
    static NetworkHelper *sharedInstance();                       // 单实例
    QByteArray uploadFile(QString urlStr, QByteArray fileByte);   // 上传文件
    QByteArray postRequest(QString urlStr);                       // POST请求
    QByteArray postRequestWithParam(QString urlStr,QString parm); // POST请求,带参数

private:
    static NetworkHelper *m_Instance;
    QNetworkAccessManager *netManager;

private slots:
    void netError(QNetworkReply::NetworkError err);

signals:
    void netError();
};

#endif // NETWORKHELPER_H
