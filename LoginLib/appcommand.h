#ifndef APPCOMMAND_H
#define APPCOMMAND_H

#include <QObject>

class AppCommand1 : public QObject
{
    Q_OBJECT
public:
    explicit AppCommand1(QObject *parent = nullptr);

signals:
    void sendSiteInfoReceived(QString webSiteURL);
    void closeDialogReceived();
public slots:
    void sendSiteInfo(QString webSiteURL); // 网页发送网站网址信息
    void closeDialog();                    // 网页调用关闭对话框
};

#endif // APPCOMMAND_H
