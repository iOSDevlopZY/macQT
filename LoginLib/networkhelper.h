#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <QObject>
#include <QWidget>
#include <QNetworkReply>
#include <QNetworkAccessManager> 
class NetworkHelper : public QObject
{
    Q_OBJECT
public:
    explicit NetworkHelper(QObject *parent = nullptr);
    ~NetworkHelper();
    QByteArray get(QString url);                     // GET方式获取数据
    QByteArray post(QString urlStr,QString Parm);    // POST方式获取数据
    QByteArray getNetPicture(QString url);           // 获取网络图片
    QByteArray postRequestWithParam(QString urlStr, QString parm);
signals:
    void netErr();
public slots:
    void netError(QNetworkReply::NetworkError err);
private:
     QNetworkAccessManager *netManager; // 网络类
};

#endif // NETWORKHELPER_H
