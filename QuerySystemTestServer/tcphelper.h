#ifndef TCPHELPER_H
#define TCPHELPER_H

#include <QObject>
#include <QTcpSocket>

class TCPHelper : public QObject
{
    Q_OBJECT
public:
    explicit TCPHelper(QObject *parent = nullptr);
    ~TCPHelper();
    bool checkIPLegality(QString IPStr);                        // 检查IP合法性
    bool checkPortLegality(QString PortStr);                    // 检查端口范围
    bool isSocketConnected();                                   // 判断Socket是否已经连接
    bool connectHost(QString IP,QString Port);                  // 连接客户端
    void disconnectConnection();                                // 断开Socket连接
    void sendCmdToClient(QString data);                         // 发送指令到客户端

private:
    QTcpSocket *connSocket;                                     // 维持连接的Socket
    bool isSocketConnect;                                       // 判断Socket是否处于连接状态
signals:
    void socketIsConnect(bool isConn);
    void socketRecvData(QString data);
public slots:
private slots:
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketReadData();
};

#endif // TCPHELPER_H
