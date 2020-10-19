#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QListWidget>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("远程查询系统测试服务端");
    helper = new TCPHelper();
    connect(helper,SIGNAL(newClientConnected(QString)),this,SLOT(newClientRecv(QString)));
    connect(helper,SIGNAL(newClientDisConnected(QString)),this,SLOT(newClientDisconnect(QString)));
    connect(helper,SIGNAL(socketRecvData(QString)),this,SLOT(newClientsocketRecvData(QString)));
    msgHelper = new MsgBoxHelper();
}

MainWindow::~MainWindow()
{
    if(helper)
        helper->deleteLater();
    if(msgHelper)
        msgHelper->deleteLater();
    delete ui;
}

/**
 * @brief 服务器开始监听
 */
void MainWindow::on_listenBtn_clicked()
{
    QString port = ui->portEdit->text();
    // 检查端口号合法性
    if(port.length() == 0)
    {
        msgHelper->showCriticalMessage("提示","端口号不能为空");
        return;
    }
    if(port.toInt() > 65535 || port.toInt() < 0)
    {
        msgHelper->showCriticalMessage("提示","端口号不合法");
        return;
    }
    bool isListenSucc = helper->startServerListen(port);
    if(!isListenSucc)
    {
        msgHelper->showCriticalMessage("提示","监听失败，请重新尝试");
        ui->listenStatusDescLabel->setText("未监听");
        return;
    }
    ui->listenBtn->setText("监听中");
    ui->listenBtn->setEnabled(false);
    ui->listenStatusLabel->setText("监听中");
}

/**
 * @brief 服务器断开监听
 */
void MainWindow::on_stopListenBtn_clicked()
{
    ui->clientListWidget->clear();
    ui->listenBtn->setText("开始监听");
    ui->listenBtn->setEnabled(true);
    ui->listenStatusLabel->setText("未监听");
    helper->stopServerListen();
}

/**
 * @brief 客户端连接
 * @param 客户端IP
 */
void MainWindow::newClientRecv(QString clientIP)
{
    QListWidgetItem *item = new QListWidgetItem(clientIP);
    ui->clientListWidget->addItem(item);
}

/**
 * @brief 客户端断开连接
 * @param 客户端IP
 */
void MainWindow::newClientDisconnect(QString clientIP)
{
    QList<QListWidgetItem*> items = ui->clientListWidget->findItems(clientIP,Qt::MatchExactly);
    for(QListWidgetItem *item : items)
    {
         ui->clientListWidget->removeItemWidget(item);
    }
}

/**
 * @brief 接收客户端指令数据
 * @param 数据
 */
void MainWindow::newClientsocketRecvData(QString data)
{
    qDebug()<<"接收到的数据为"<<data;
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(),&error);
    if(error.error == QJsonParseError::NoError)
    {
        QJsonObject obj = doc.object();
        if(obj["Type"] == "ClientSupportedQueryAndSettings")
        {
            // 发送支持的类型
            sendSupportType();
        }
    }
    else
    {
        msgHelper->showCriticalMessage("提示","接收JSON数据解析失败!");
;    }
}

/**
 * @brief 发送支持的类型
 * {"Type":"ClientSupportedQueryAndSettingsReply","Code":"#成功为200，失败为-1#","Msg":"#只有失败时，写明失败原因#","Data":"{"Queires":[{},{},{}],"Settings":[{},{},{}]}"}
 * {"Type"："#查询项或者设置项的名称#","Desc":"#设置项或者查询项的名称#","Data"：{"XXX":"XXX",...}}
 */
void MainWindow::sendSupportType()
{
    QJsonObject Obj;
    Obj.insert("Type","ClientSupportedQueryAndSettingsReply");
    Obj.insert("Code","200");
    Obj.insert("Msg","");
    // data
    QJsonObject dataObj;
    QJsonArray queryArray;
    // query里面的数组
    // 查询项1
    QJsonObject queryObject1;
    queryObject1.insert("Type","Query1");
    queryObject1.insert("Desc","查询项1");
    // 查询项2
    QJsonObject queryObject2;
    queryObject2.insert("Type","Query2");
    queryObject2.insert("Desc","查询项2");
    QJsonObject queryObject2Data;
    queryObject2Data.insert("TestParam1","");
    queryObject2Data.insert("TestParam2","");
    queryObject2.insert("Data",queryObject2Data);

    queryArray.append(queryObject1);
    queryArray.append(queryObject2);

    // setting里面的数组
    QJsonArray settingArray;
    // 查询项1
    QJsonObject settingObject1;
    settingObject1.insert("Type","Setting1");
    settingObject1.insert("Desc","设置项1");
    // 查询项2
    QJsonObject settingObject2;
    settingObject2.insert("Type","Setting2");
    settingObject2.insert("Desc","设置项2");
    QJsonObject settingObject2Data;
    settingObject2Data.insert("TestParam1","");
    settingObject2Data.insert("TestParam2","");
    settingObject2.insert("Data",settingObject2Data);
    // 查询项3
    QJsonObject settingObject3;
    settingObject3.insert("Type","Setting3");
    settingObject3.insert("Desc","设置项3");
    QJsonObject settingObject3Data;
    settingObject3Data.insert("TestParam1","");
    settingObject3Data.insert("TestParam2","");
    settingObject3Data.insert("TestParam3","");
    settingObject3.insert("Data",settingObject3Data);

    settingArray.append(settingObject1);
    settingArray.append(settingObject2);
    settingArray.append(settingObject3);

    dataObj.insert("Queires",queryArray);
    dataObj.insert("Settings",settingArray);
    QJsonDocument dataDoc = QJsonDocument(dataObj);
    QString str(dataDoc.toJson());
    QByteArray encodeData = str.toUtf8().toBase64();

    Obj.insert("Data",QString(encodeData));
    QJsonDocument ObjDoc = QJsonDocument(Obj);
    helper->sendDataToQuerySystem(ObjDoc.toJson());
}
