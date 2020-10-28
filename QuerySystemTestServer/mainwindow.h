#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcphelper.h"
#include "msgboxhelper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listenBtn_clicked();
    void on_stopListenBtn_clicked();
    void newClientRecv(QString clientIP);
    void newClientDisconnect(QString clientIP);
    void newClientsocketRecvData(QString data);


private:
    Ui::MainWindow *ui;
    TCPHelper *helper;
    MsgBoxHelper *msgHelper;

    void sendSupportType();
    void sendSettingMsg(bool isSuccess,QString type);
    void sendQueryMsg(bool isSuccess, QString type, QString data);
};
#endif // MAINWINDOW_H
