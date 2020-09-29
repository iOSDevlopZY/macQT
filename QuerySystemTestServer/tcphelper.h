#ifndef TCPHELPER_H
#define TCPHELPER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

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
    bool startServerListen(QString port);                       // 开启服务端监听
    void stopServerListen();                                    // 停止服务端监听
    void sendDataToQuerySystem(QByteArray dataArr);             // 向查询系统发送数据

private:
    QTcpSocket *connSocket;                                     // 维持连接的Socket
    QTcpServer *server;
    QTcpSocket *clientSocket;
    bool isSocketConnect;                                       // 判断Socket是否处于连接状态
    QList<QTcpSocket*> tcpSocketConnetList;                     // 连接的客户端列表
signals:
    void socketIsConnect(bool isConn);
    void socketRecvData(QString data);
    void newClientConnected(QString IP);
    void newClientDisConnected(QString IP);

public slots:
private slots:
    void socketStateChanged(QAbstractSocket::SocketState state);
    void newConnectionComing();
    void socketReadData();
    void clientSocketReadData();
    void clientSocketDisconnect();
};

#endif // TCPHELPER_H
