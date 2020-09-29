#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("远程查询系统测试服务端");
    helper = new TCPHelper();
    connect(helper,SIGNAL(newClientConnected(QString)),this,SLOT(newClientRecv(QString)));
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
