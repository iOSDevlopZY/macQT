#include "appcommand.h"
#include <QDebug>
#include <QCoreApplication>
#include <QSettings>

AppCommand1::AppCommand1(QObject *parent) : QObject(parent)
{

}

///
/// \brief 网页向本地发送网址信息
/// \param 网址
///
void AppCommand1::sendSiteInfo(QString webSiteURL)
{
    emit sendSiteInfoReceived(webSiteURL);
}

///
/// \brief 关闭对话框
///
void AppCommand1::closeDialog()
{
    emit closeDialogReceived();
}


