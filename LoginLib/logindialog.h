#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include "clicklabel.h"
#include "inihelper.h"
#include "settingdialog.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QMouseEvent>
#include "customlineedit.h"
#include "networkhelper.h"

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    ClickLabel *minimizeLabel; // 最小化
    ClickLabel *closeLabel;    // 关闭
    QLabel *logoLabel;         // logo

    CustomLineEdit *userNameEdit;   // 用户名输入框
    CustomLineEdit *userPasswdEdit; // 密码输入框
    ClickLabel *loginLabel;    // 登录按钮
//    ClickLabel *settingLabel;  // 设置按钮
    SettingDialog *settingDialog; // 设置界面
    INIHelper1 *iniHelper;      // INI帮助类
    QPixmap settingMap;        // 设置位图
    QNetworkAccessManager *NetWorkManager; // 网络类
    bool m_Drag; // 是否鼠标拖动
    QPoint m_position;
    NetworkHelper *netHelper;
    QString PlatformCode;
    bool isOfficalEnv;
    QString anymedPrefixURL;
    QString choosePlatformURL;

    void initTool();           // 工具类初始化
    void setMap();             // 设置位图
    void setupUI();            // 设置UI
    void getLoginInfo(QString urlStr, QString Parm);  // 获取登录信息
    void saveAccountData(QJsonObject dataObj); // 存储账号信息
    void startupApplication(); // 启动程序
    bool isDirectLogin(QString userJID);

    void ShowCriticalMessage(QString title,QString message);

protected:
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent (QMouseEvent *e);
    void mouseMoveEvent (QMouseEvent *e);
    void mouseReleaseEvent (QMouseEvent *e);

private slots:
    void minimizeClick(); // 最小化点击事件
    void closeClick();    // 关闭按钮事件
    void loginClick();    // 登录点击事件
    void settingClick();  // 设置点击事件
    void loginerror(QNetworkReply::NetworkError err); // 登陆出错
    void startApp();
signals:

public slots:
};

#endif // LOGINDIALOG_H
