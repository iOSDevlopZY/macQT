#include "tcphelper.h"
#include <QDebug>

/**
 * @brief int转字节
 * @param int数据
 * @return
 */
QByteArray  intToByte(int i)
{
    QByteArray abyte0;
    abyte0.resize(4);
    abyte0[0] = (uchar)  (0x000000ff & i);
    abyte0[1] = (uchar) ((0x0000ff00 & i) >> 8);
    abyte0[2] = (uchar) ((0x00ff0000 & i) >> 16);
    abyte0[3] = (uchar) ((0xff000000 & i) >> 24);
    return abyte0;
}

TCPHelper::TCPHelper(QObject *parent) : QObject(parent)
{
    connSocket = new QTcpSocket();
    clientSocket = new QTcpSocket();
    isSocketConnect = false;
    connect(connSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,
            SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(connSocket, SIGNAL(readyRead()), this, SLOT(socketReadData()));

    server = new QTcpServer();
    server->setMaxPendingConnections(500);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnectionComing()));
}

TCPHelper::~TCPHelper()
{
    if(connSocket)
    {
        if(isSocketConnect)
            connSocket->disconnectFromHost();
        connSocket->deleteLater();
    }
    if(server)
    {
        if(server->isListening())
            server->close();
        server->deleteLater();
    }
    tcpSocketConnetList.clear();
}

/**
 * @brief 检查IP合法性
 */
bool TCPHelper::checkIPLegality(QString IPStr)
{
    QStringList IPList = IPStr.split(".");
    if(IPList.count() == 4 && IPList.at(3) != "")
        return true;
    return false;
}

/**
 * @brief 检查端口范围
 * @param 端口号
 * @return
 */
bool TCPHelper::checkPortLegality(QString PortStr)
{
    int port = PortStr.toInt();
    if(port < 0 || port > 65535)
        return false;
    return true;
}

/**
 * @brief 判断Socket是否已经连接
 * @return
 */
bool TCPHelper::isSocketConnected()
{
    return isSocketConnect;
}

/**
 * @brief 连接客户端
 * @param IP
 * @param 端口号
 * @return
 */
bool TCPHelper::connectHost(QString IP, QString Port)
{
    connSocket->connectToHost(IP,Port.toInt());
    bool connected = connSocket->waitForConnected();
    return connected;
}

/**
 * @brief 断开Socket连接
 */
void TCPHelper::disconnectConnection()
{
    connSocket->disconnectFromHost();
}

/**
 * @brief 发送指令
 * @param 指令
 */
void TCPHelper::sendCmdToClient(QString data)
{
    int length=data.length();
    QByteArray lengthArr=intToByte(length);
    if(isSocketConnect)
    {
        QByteArray array;
        array.append(lengthArr);
        array.append(data.toUtf8());
        connSocket->write(array);
        connSocket->waitForBytesWritten();
    }
}

/**
 * @brief 开启服务器监听
 */
bool TCPHelper::startServerListen(QString port)
{
    return server->listen(QHostAddress::Any,port.toInt());
}

/**
 * @brief 停止服务端监听
 */
void TCPHelper::stopServerListen()
{
    if(server->isListening())
        server->close();
    tcpSocketConnetList.clear();
}

/**
 * @brief 向查询系统发送数据
 * @param 数组
 */
void TCPHelper::sendDataToQuerySystem(QByteArray dataArr)
{
    int length=dataArr.length();
    QByteArray lengthArr=intToByte(length);
    QByteArray array;
    array.append(lengthArr);
    array.append(dataArr);
    clientSocket->write(array);
    clientSocket->waitForBytesWritten();
}

/**
 * @brief Socket接收数据
 */
void TCPHelper::socketReadData()
{
    char *data = new char[4];

    clientSocket->read(data,4);

    int len = *(int *)data;
    if(len>0)
    {
         int shellRead = clientSocket->bytesAvailable();
         int readLen = 0;
         char *data2 = new char[shellRead+1];
         memset(data2,0,shellRead+1);
         int readyRead = clientSocket->read(data2,shellRead);
         readLen += readyRead;
         while(readLen < shellRead)
         {
             char *data3 = new char[shellRead - readLen];
             int readyRead = clientSocket->read(data3,shellRead - readLen);
             memcpy(data2+readLen,data3,readyRead);
             readLen += readyRead;
             delete [] data3;
         }
         QString dataStr(data2);
         emit socketRecvData(dataStr);
         delete [] data2;

    }

    delete [] data;
}

/**
 * @brief 服务端接收客户端Socket数据
 */
void TCPHelper::clientSocketReadData()
{
    char *data = new char[4];

    clientSocket->read(data,4);

    int len = *(int *)data;
    if(len>0)
    {
         int shellRead = clientSocket->bytesAvailable();
         int readLen = 0;
         char *data2 = new char[shellRead+1];
         memset(data2,0,shellRead+1);
         int readyRead = clientSocket->read(data2,shellRead);
         readLen += readyRead;
         while(readLen < shellRead)
         {
             char *data3 = new char[shellRead - readLen];
             int readyRead = clientSocket->read(data3,shellRead - readLen);
             memcpy(data2+readLen,data3,readyRead);
             readLen += readyRead;
             delete [] data3;
         }
         QString dataStr(data2);
         emit socketRecvData(dataStr);
         delete [] data2;

    }

    delete [] data;
}

/**
 * @brief 客户端退出
 */
void TCPHelper::clientSocketDisconnect()
{
    tcpSocketConnetList.removeOne(clientSocket);
    emit newClientDisConnected(clientSocket->peerAddress().toString());
}

/**
 * @brief 有新客户端加入
 * @param 客户端
 */
void TCPHelper::newConnectionComing()
{
    clientSocket = server->nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(clientSocketReadData()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(clientSocketDisconnect()));
    tcpSocketConnetList.append(clientSocket);
    emit newClientConnected(clientSocket->peerAddress().toString());
}

/**
 * @brief Socket连接状态
 * @param 连接状态
 */
void TCPHelper::socketStateChanged(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::ConnectedState:
        isSocketConnect = true;
        break;
    case  QAbstractSocket::UnconnectedState:
        isSocketConnect = false;
        break;
    case  QAbstractSocket::HostLookupState:
        isSocketConnect = false;
        break;
    case  QAbstractSocket::ConnectingState:
        isSocketConnect = false;
        break;
    case  QAbstractSocket::BoundState:
        isSocketConnect = false;
        break;
    case  QAbstractSocket::ListeningState:
        isSocketConnect = false;
        break;
    case  QAbstractSocket::ClosingState:
        isSocketConnect = false;
        break;
    default:
        break;
    }
    emit socketIsConnect(isSocketConnect);
}


