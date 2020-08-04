#include "loginlib.h"
#include <QApplication>
#include <QCoreApplication>
#include <QResource>
#include <QDebug>
#include "logindialog.h"

LoginLib *LoginLib::m_Instance = NULL;
LoginLib::LoginLib()
{
    // 加载资源文件
    QString appPath = QCoreApplication::applicationDirPath()+"/Skin/Skin.rcc";
    bool regResult = QResource::registerResource(appPath);
    if(!regResult)
    {
        qWarning()<<QString::fromLocal8Bit("加载资源文件失败");
    }

}

LoginLib::~LoginLib()
{
    if(m_Instance)
        delete m_Instance;
}

///
/// \brief 单实例
/// \return
///
LoginLib *LoginLib::Instance()
{
    if(m_Instance == NULL)
    {
        m_Instance = new LoginLib();
    }
    return m_Instance;
}

///
/// \brief 展示对话框
///
void LoginLib::showLoginDialog()
{
    LoginDialog *dialog = new LoginDialog();
    dialog->exec();
    dialog->deleteLater();
}
