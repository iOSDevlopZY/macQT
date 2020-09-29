#include "msgboxhelper.h"

MsgBoxHelper::MsgBoxHelper(QObject *parent) : QObject(parent)
{

}

/**
 * @brief MsgBoxHelper::showInformationMsg
 * @param 标题
 * @param 消息
 */
void MsgBoxHelper::showInformationMsg(QString title, QString msg)
{
    QMessageBox infoMsgBox(QMessageBox::Information,title,msg);
    infoMsgBox.setStandardButtons(QMessageBox::Yes);
    infoMsgBox.setButtonText(QMessageBox::Yes,QString::fromLocal8Bit("确定"));
    infoMsgBox.exec();
}

/**
 * @brief MsgBoxHelper::showChooseMsg
 * @param 标题
 * @param 消息
 */
int MsgBoxHelper::showChooseMsg(QString title, QString msg)
{
    QMessageBox exitMsgBox(QMessageBox::Question,title,msg);
    exitMsgBox.setStandardButtons (QMessageBox::Yes|QMessageBox::No);
    exitMsgBox.setButtonText (QMessageBox::Yes,QString::fromLocal8Bit ("确定"));
    exitMsgBox.setButtonText (QMessageBox::No,QString::fromLocal8Bit ("取消"));
    return exitMsgBox.exec ();
}


/**
 * @brief 显示错误信息
 * @param 标题
 * @param 信息
 */
void MsgBoxHelper::showCriticalMessage(QString title,QString message)
{
    QMessageBox warnMsgBox(QMessageBox::Critical,title,message);
    warnMsgBox.setStandardButtons (QMessageBox::Yes);
    warnMsgBox.setButtonText (QMessageBox::Yes,QString::fromLocal8Bit ("确定"));
    warnMsgBox.exec();
}
