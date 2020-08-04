#include "chooseplatformdialog.h"
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QDir>
#include <QWebChannel>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>

ChoosePlatformDialog::ChoosePlatformDialog(QWidget *parent):QDialog(parent)
{
    Q_UNUSED(parent);
    iniHelper = new INIHelper1();
    this->setWindowTitle(QString::fromLocal8Bit("平台选择"));
    setupUI();
//    this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
//     // 固定窗体大小
//    showFullScreen();
}

ChoosePlatformDialog::~ChoosePlatformDialog()
{
    if(iniHelper)
        delete iniHelper;
}

/**
 * @brief 设置UI
 */
void ChoosePlatformDialog::setupUI()
{
    m_Webview = new QWebEngineView(this);

    m_Webview->setZoomFactor(1);

    //清除缓存
    clearCache();

    loadingProgressBar = new QProgressBar(this);

    loadingProgressBar->setOrientation(Qt::Horizontal);

    loadingProgressBar->setMinimum(0);

    loadingProgressBar->setMaximum(100);

    loadingProgressBar->setValue(0);

    loadingProgressBar->hide();

    loadingProgressBar->setAlignment(Qt::AlignCenter);

    connect(m_Webview,SIGNAL(loadProgress(int)),this,SLOT(webViewLoadProgress(int)));

    command = new AppCommand1(this);

    connect(command,SIGNAL(sendSiteInfoReceived(QString)),this,SLOT(webSiteInfoReceived(QString)));

    connect(command,SIGNAL(closeDialogReceived()),this,SLOT(closeDialog()));

    QWebChannel *channel = new QWebChannel(this);

    channel->registerObject("AppCommand",command);

    m_Webview->page()->setWebChannel(channel);
}

///
/// \brief 清除缓存
///
void ChoosePlatformDialog::clearCache()
{
    QWebEngineProfile * engineProfile = m_Webview->page()->profile();
    engineProfile->clearHttpCache(); // 删除缓存

    QWebEngineCookieStore *cookie = engineProfile->cookieStore();
    cookie->deleteAllCookies();      // 删除cookie

    QString cachePath = engineProfile->cachePath();
    QDir cachePathDir(cachePath);

    if (cachePathDir.exists())
    {
        bool rlt = cachePathDir.removeRecursively();

        if (!rlt)
        {
           qDebug() << QStringLiteral("clear cache failed");
        }
    }
}

///
/// \brief 根据JID设置URL
/// \param url
///
void ChoosePlatformDialog::setWebUrl(QString url)
{
    m_Webview->page()->load(QUrl(url));
}

///
/// \brief 收到网址信息
/// \param 信息
///
void ChoosePlatformDialog::webSiteInfoReceived(QString info)
{
    qDebug()<<"+++++++++++++++++++++++++++++++++++++++websiteinfo : "<<info;
    iniHelper->writeAccountInfo("AccountInfo/theurl",info);
    emit sureClick();
    this->accept();
}

///
/// \brief 收到网页关闭对话框
///
void ChoosePlatformDialog::closeDialog()
{
   this->close();
}

///
/// \brief 网页加载
/// \param 值
///
void ChoosePlatformDialog::webViewLoadProgress(int value)
{
    loadingProgressBar->show();

    loadingProgressBar->setValue(value);

    if(value == 100)
    {
        loadingProgressBar->hide();
    }
}

void ChoosePlatformDialog::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    QRect desktopRect = QApplication::desktop()->availableGeometry();

    int iTitleBarHeight = style()->pixelMetric(QStyle::PM_TitleBarHeight);  // 获取标题栏高度

    setGeometry(0, 0 , desktopRect.width(), desktopRect.height());  // 设置窗体充满桌面客户区

    setFixedSize(desktopRect.width(), desktopRect.height()+iTitleBarHeight);
    m_Webview->setGeometry(0,0,this->width(),this->height());
    loadingProgressBar->setGeometry(this->width()*0.4,this->height()*0.485,this->width()*0.2,this->height()*0.03);
}


