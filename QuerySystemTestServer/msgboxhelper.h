#ifndef MSGBOXHELPER_H
#define MSGBOXHELPER_H

#include <QObject>
#include <QMessageBox>

class MsgBoxHelper : public QObject
{
    Q_OBJECT
public:
    explicit MsgBoxHelper(QObject *parent = nullptr);
    void showInformationMsg(QString title,QString msg);       // 显示信息
    int showChooseMsg(QString title,QString msg);             // 显示选择消息
    void showCriticalMessage(QString title, QString message);  // 显示错误消息
signals:

public slots:
};

#endif // MSGBOXHELPER_H
