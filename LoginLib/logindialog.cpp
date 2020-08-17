#include "logindialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include "chooseplatformdialog.h"
#include <QCoreApplication>

#define WINDOW_WIDTH this->size().width()
#define WINDOW_HEIGHT this->size().height()
#define MEDAPI_OFFICIAL "https://sino-med.net:466"
#define MEDAPI_TEST "https://cs1.sino-med.net:466"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle(QString::fromLocal8Bit("登录"));
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("background-image:url(:/images/background)");
    this->setFixedSize(QSize(400,500));

    m_Drag = false;
    PlatformCode = "";
    isOfficalEnv = false;
    anymedPrefixURL = "";
    choosePlatformURL = "";

    initTool();
    // 设置位图
    setMap();
    // 设置UI
    setupUI();
}

LoginDialog::~LoginDialog()
{
    if(NetWorkManager)
        delete NetWorkManager;
    if(iniHelper)
        delete iniHelper;
    if(netHelper)
        delete netHelper;
}

/**
 * @brief 工具类初始化
 */
void LoginDialog::initTool()
{
    netHelper = new NetworkHelper();
    iniHelper = new INIHelper1();
    NetWorkManager = new QNetworkAccessManager();
}

/**
 * @brief 设置位图
 */
void LoginDialog::setMap()
{
    settingMap.load(":/images/settings");
}

/**
 * @brief 设置UI
 */
void LoginDialog::setupUI()
{
    QFont labelFont("Microsoft Yahei",15);
    QFont editFont("Microsoft Yahei",12);
    // 最小化按钮
    minimizeLabel = new ClickLabel(this);
    minimizeLabel->setAlignment(Qt::AlignCenter);
    minimizeLabel->setPixmap(QPixmap(":/images/minimize"));
    minimizeLabel->setScaledContents (true);
    minimizeLabel->setStyleSheet ("background:transparent");
    connect(minimizeLabel,SIGNAL(Clicked()),this,SLOT(minimizeClick()));

    //关闭按钮
    closeLabel = new ClickLabel(this);
    closeLabel->setAlignment(Qt::AlignCenter);
    closeLabel->setPixmap(QPixmap(":/images/close"));
    closeLabel->setScaledContents (true);
    closeLabel->setStyleSheet ("background:transparent");
    connect(closeLabel,SIGNAL(Clicked()),this,SLOT(closeClick()));

    // logo
    QPixmap map(":/images/logo");
    logoLabel = new QLabel(this);
    logoLabel->setStyleSheet("background:transparent");
    logoLabel->setPixmap(map);
    logoLabel->setScaledContents (true);
    logoLabel->setAlignment(Qt::AlignCenter);

    // 用户名输入
    userNameEdit = new CustomLineEdit(this);
    userNameEdit->setStyleSheet("background:white");
    userNameEdit->entryEdit->setPlaceholderText(QString::fromLocal8Bit("账户名"));
    userNameEdit->entryEdit->setFont(editFont);
    userNameEdit->logoLabel->setPixmap (QPixmap(":/images/username"));
    QString loginName = iniHelper->readLoginInfo("Login/Username");
    if(loginName.length () > 0)
    {
        userNameEdit->entryEdit->setText(loginName);
    }

    // 用户密码输入
    userPasswdEdit = new CustomLineEdit(this);
    userPasswdEdit->entryEdit->setEchoMode(QLineEdit::Password);
    userPasswdEdit->setStyleSheet("background:white");
    userPasswdEdit->entryEdit->setPlaceholderText(QString::fromLocal8Bit("密码"));
    userPasswdEdit->entryEdit->setFont(editFont);
    userPasswdEdit->logoLabel->setPixmap (QPixmap(":/images/password"));
    // 登录
    loginLabel = new ClickLabel(this);
    loginLabel->setAlignment(Qt::AlignCenter);
    loginLabel->setScaledContents (true);
    loginLabel->setStyleSheet("background:transparent");
    loginLabel->setPixmap(QPixmap(":/images/logining"));
    connect(loginLabel,SIGNAL(Clicked()),this,SLOT(loginClick()));

//    settingLabel = new ClickLabel(this);
//    settingLabel->setAlignment(Qt::AlignCenter);
//    settingLabel->setStyleSheet("background:transparent");
//    settingLabel->setPixmap(settingMap);
//    settingLabel->setScaledContents(true);
//    connect(settingLabel,SIGNAL(Clicked()),this,SLOT(settingClick()));
}

/**
 * @brief 登录点击事件
 */
void LoginDialog::loginClick()
{
    if(userNameEdit->entryEdit->text().length() > 0 && userPasswdEdit->entryEdit->text().length() > 0 )
    {
        if(userNameEdit->entryEdit->text().toLower().startsWith("mdcbctest"))
            isOfficalEnv = false;
        else
            isOfficalEnv = true;

            loginLabel->setPixmap(QPixmap(":/images/login"));
            QString param = "";
            QString urlStr = "";
               if(isOfficalEnv)
               {
                   param = "wsJid=__workstation_b8975a1489c1@im.sino-med.net&customKey=Configuration";
               }
               else
               {
                   param = "wsJid=__workstation_b8975a1489c1@im.sino-med.net&customKey=ConfigurationTest";
               }
               QString url = "http://121.42.48.71:8021/cms-api/LanWorkStation/GetWSCustomdata";

               QByteArray res = netHelper->postRequestWithParam(url,param);
               QJsonParseError error;
               QJsonDocument doc = QJsonDocument::fromJson(res,&error);
               if(error.error == QJsonParseError::NoError)
               {
                   QJsonObject obj = doc.object();

                   QString dataStr = obj["data"].toString();

                   QJsonDocument doc1 = QJsonDocument::fromJson(dataStr.toUtf8(),&error);
                   if(error.error == QJsonParseError::NoError)
                   {
                       QJsonObject dataObj = doc1.object();
                       urlStr = dataObj["LoginUrl"].toString();
                       anymedPrefixURL = dataObj["GetPlatformNumUrl"].toString();
                       choosePlatformURL = dataObj["SelectPlatformUrl"].toString();
                       qDebug()<<QString::fromLocal8Bit("读取的登录接口为：")<<urlStr;
                       qDebug()<<QString::fromLocal8Bit("读取的平台接口前缀为：")<<anymedPrefixURL;
                       qDebug()<<QString::fromLocal8Bit("读取的选择平台网页接口为：")<<choosePlatformURL;
                   }
               }
            qDebug()<<QString::fromLocal8Bit("读取的URL为：")<<urlStr;
            QString parm = QString("name=%1&password=%2").arg(userNameEdit->entryEdit->text()).arg(userPasswdEdit->entryEdit->text());
            qDebug()<<QString::fromLocal8Bit("传入的参数：")<<urlStr;
            getLoginInfo(urlStr,parm);
    }
    else
    {
        ShowCriticalMessage(QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("用户名和密码都不能为空"));
    }
}

/**
 * @brief 设置点击事件
 */
void LoginDialog::settingClick()
{
    settingDialog = new SettingDialog(this);
    settingDialog->resize(QSize(WINDOW_WIDTH *0.6,WINDOW_HEIGHT*0.4));
    settingDialog->exec();
    settingDialog->deleteLater();
}

/**
 * @brief 登录出错
 */
void LoginDialog::loginerror(QNetworkReply::NetworkError err)
{
    if(err!=QNetworkReply::NoError)
    {
        ShowCriticalMessage(QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("无法登录，请检查网络"));
        loginLabel->setPixmap(QPixmap(":/images/logining"));
    }
}

/**
 * @brief 获取登录信息
 * @param url
 */
void LoginDialog::getLoginInfo(QString urlStr,QString Parm)
{
    QString UserJid = "";
    QString nickName = "";
    QUrl url(urlStr);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postArray;
    postArray.append(Parm);

    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.length());
    qDebug()<<QString::fromLocal8Bit("begin post");
    QNetworkReply *Reply = NetWorkManager->post(request, postArray);
    connect (Reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loginerror(QNetworkReply::NetworkError)));
    QEventLoop eventLoop;
    connect(NetWorkManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    disconnect(NetWorkManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    qDebug()<<QString::fromLocal8Bit("return post");
    // 解析返回结果
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(Reply->readAll(), &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
       loginLabel->setPixmap(QPixmap(":/images/logining"));
       ShowCriticalMessage(QString::fromLocal8Bit ("提示"),QString::fromLocal8Bit ("服务器返回数据错误"));
        qWarning() << QString::fromLocal8Bit ("解析接口返回数据出错：") << Reply->errorString();
        Reply->deleteLater();
        return;
    }

    QJsonObject returnObj = jsonDoc.object();
    if(returnObj["code"].toString() == "200")
    {
        // 将数据信息存储下来
//        QJsonObject dataObj = returnObj["accountdata"].toObject();
//        saveAccountData(dataObj);
        QJsonObject dataObj = returnObj["data"].toObject();
        UserJid = dataObj["userJID"].toString();
        nickName = dataObj["nickName"].toString();
        saveAccountData(dataObj);
    }
    else
    {
        loginLabel->setPixmap(QPixmap(":/images/logining"));
        ShowCriticalMessage(QString::fromLocal8Bit("提示"),returnObj["msg"].toString());
        Reply->deleteLater();
        return;
    }
    Reply->deleteLater();
    // 存入登录用户名
    iniHelper->writeLoginInfo("Login/Username",userNameEdit->entryEdit->text());

    bool isDirectL = isDirectLogin(UserJid);
    if(isDirectL)
    {
        // 获取URL
        QString url = QString("%1/api/AnymedApi/GetModulesBySubPlatform?UserJID=%2&PlatformCode=%3")
                .arg(anymedPrefixURL).arg(UserJid).arg(PlatformCode);
        QByteArray dataArray = netHelper->get(url);
        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(dataArray, &json_error));
        if(json_error.error != QJsonParseError::NoError)
        {
            qWarning() << QString::fromLocal8Bit ("解析平台URL接口返回数据出错：");
        }

        QJsonObject returnObj = jsonDoc.object();
        QJsonArray dataArr = returnObj["data"].toArray();
        for(int i =0;i<dataArr.count();i++)
        {
            QJsonObject obj = dataArr[i].toObject();
            // 直播助手
            if(obj.contains("AppType") && obj["AppType"] == "Live")
            {
                // 保存URL
                QString url = obj["ModuleUrl"].toString();
                iniHelper->writeAccountInfo("AccountInfo/theurl",url);
                break;
            }
        }

        // 直接启动程序
        startApp();
    }
    else
    {
        // 弹出选择平台对话框
        ChoosePlatformDialog *chooseDialog = new ChoosePlatformDialog(this);
        QString webURL = QString("%1?UserJID=%2").arg(choosePlatformURL).arg(UserJid);

        chooseDialog->setWebUrl(webURL);
        connect (chooseDialog,SIGNAL(sureClick()),this,SLOT(startApp()));
        chooseDialog->exec();
    //    chooseDialog->deleteLater();

        loginLabel->setPixmap(QPixmap(":/images/logining"));
    }

}

/**
 * @brief 启动程序
 */
void LoginDialog::startApp()
{
    // 记录当前环境
    QString flagPath = QCoreApplication::applicationDirPath()+"/flag.txt";
    QFile file(flagPath);
    if(file.open(QIODevice::ReadWrite|QIODevice::Text))
    {
        if(isOfficalEnv)
            file.write("True");
        else
            file.write("False");
    }
    file.close();
    // 成功才能启动程序
    startupApplication();
}

/**

 * @brief 存储账号信息
 * @param 数据JSON
 */
void LoginDialog::saveAccountData(QJsonObject dataObj)
{
    QString account = "";//dataObj["account"].toString();
    QString accountcategory = "";//dataObj["accountcategory"].toString();
    QString departmentname = "";//dataObj["departmentname"].toString();
    QString hospitalname = "";//dataObj["hospitalname"].toString();
    QString id = "";//dataObj["id"].toString();
    QString imageurl = "";//dataObj["imageurl"].toString();
    QString live = "";//dataObj["live"].toString();
    QString medicalconjoinedname = "";//dataObj["medicalconjoinedname"].toString();
    QString nickname = dataObj["nickName"].toString();
    QString ondemand = "";//dataObj["ondemand"].toString();
    QString pecialcolumn = "";//dataObj["pecialcolumn"].toString();
    QString telephone = "";//dataObj["telephone"].toString();
    QString title = "";//dataObj["title"].toString();
    QString titlename = "";//dataObj["titlename"].toString();
    QString userjid = dataObj["userJID"].toString();
    QString liveChannel = "";//dataObj["liveChannel"].toString();
    QString streamUrl ="";// dataObj["streamUrl"].toString();
    QString theurl = "";//dataObj["theurl"].toString();

    iniHelper->writeAccountInfo("AccountInfo/account",account);
    iniHelper->writeAccountInfo("AccountInfo/accountcategory",accountcategory);
    iniHelper->writeAccountInfo("AccountInfo/departmentname",departmentname);
    iniHelper->writeAccountInfo("AccountInfo/hospitalname",hospitalname);
    iniHelper->writeAccountInfo("AccountInfo/id",id);
    iniHelper->writeAccountInfo("AccountInfo/imageurl",imageurl);
    iniHelper->writeAccountInfo("AccountInfo/live",live);
    iniHelper->writeAccountInfo("AccountInfo/medicalconjoinedname",medicalconjoinedname);
    iniHelper->writeAccountInfo("AccountInfo/nickname",nickname);
    iniHelper->writeAccountInfo("AccountInfo/ondemand",ondemand);
    iniHelper->writeAccountInfo("AccountInfo/pecialcolumn",pecialcolumn);
    iniHelper->writeAccountInfo("AccountInfo/telephone",telephone);
    iniHelper->writeAccountInfo("AccountInfo/title",title);
    iniHelper->writeAccountInfo("AccountInfo/titlename",titlename);
    iniHelper->writeAccountInfo("AccountInfo/userjid",userjid);
    iniHelper->writeAccountInfo("AccountInfo/liveChannelNum",liveChannel);
    iniHelper->writeAccountInfo("AccountInfo/streamUrl",streamUrl);
    iniHelper->writeAccountInfo("AccountInfo/theurl",theurl);
}

/**
 * @brief 启动程序
 */
void LoginDialog::startupApplication()
{
//    QString startAppPath = QCoreApplication::applicationDirPath()+"/MedBroadcast.exe";
//    QProcess startProcess;
//    startProcess.startDetached(startAppPath);
//    startProcess.close();
//    qDebug()<<QString::fromLocal8Bit("启动了OBS");
    this->accept();
}

void LoginDialog::ShowCriticalMessage(QString title, QString message)
{
    QMessageBox warnMsgBox(QMessageBox::Critical,title,message);
    warnMsgBox.setStandardButtons (QMessageBox::Yes);
    warnMsgBox.setButtonText (QMessageBox::Yes,QString::fromLocal8Bit ("确定"));
    warnMsgBox.exec ();
}


/**
 * @brief 最小化点击事件
 */
void LoginDialog::minimizeClick()
{
    this->showMinimized();
}

/**
 * @brief 关闭按钮事件
 */
void LoginDialog::closeClick()
{
    QMessageBox exitMsgBox(QMessageBox::Question,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("确定要退出?"));
    exitMsgBox.setStandardButtons (QMessageBox::Yes|QMessageBox::No);
    exitMsgBox.setButtonText (QMessageBox::Yes,QString::fromLocal8Bit ("确定"));
    exitMsgBox.setButtonText (QMessageBox::No,QString::fromLocal8Bit ("取消"));

    int button = exitMsgBox.exec ();
    if(button == QMessageBox::Yes)
    {
        exit(0);
    }
}

/**
 * @brief 是否直接登录
 * @param userJID
 * @return
 */
bool LoginDialog::isDirectLogin(QString userJID)
{
    // 获取平台数量
    QString url = QString("%1/api/AnymedApi/GetMySubPlatform?UserJID=%2").arg(anymedPrefixURL).arg(userJID);
    QByteArray dataArray = netHelper->get(url);
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(dataArray, &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        qWarning() << QString::fromLocal8Bit ("解析平台接口数量接口返回数据出错：");
        return false;
    }

    QJsonObject returnObj = jsonDoc.object();
    QJsonArray dataArr = returnObj["data"].toArray();

    // 如果有两个&&其中一个是私有,取私有的platformcode
    if(dataArr.count() == 2)
    {
        for(int i=0;i<dataArr.count();i++)
        {
            QJsonObject obj = dataArr[i].toObject();
            // 是私有的
            if(!obj.contains("AppliedToAll") ||
               (obj.contains("AppliedToAll") &&
                obj["AppliedToAll"].toString() != "是"))
            {
                PlatformCode = obj["PlatformCode"].toString();
                return true;
            }
        }
    }
    return false;
}
/**
 * @brief 界面布局事件
 * @param QResizeEvent
 */
void LoginDialog::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    minimizeLabel->setGeometry(QRect(WINDOW_WIDTH*0.85,WINDOW_HEIGHT*0.02,WINDOW_WIDTH*0.04,WINDOW_WIDTH*0.04));
    closeLabel->setGeometry(QRect(WINDOW_WIDTH*0.93,WINDOW_HEIGHT*0.02,WINDOW_WIDTH*0.04,WINDOW_WIDTH*0.04));
    logoLabel->setGeometry(QRect(WINDOW_WIDTH*0.5 - WINDOW_HEIGHT*0.32/298*330/2,WINDOW_HEIGHT*0.1,WINDOW_HEIGHT*0.32/298*330,WINDOW_HEIGHT*0.32));
    userNameEdit->setGeometry(QRect(WINDOW_WIDTH*0.15,WINDOW_HEIGHT*0.48,WINDOW_WIDTH*0.7,WINDOW_HEIGHT*0.09));
    userPasswdEdit->setGeometry(QRect(WINDOW_WIDTH*0.15,WINDOW_HEIGHT*0.61,WINDOW_WIDTH*0.7,WINDOW_HEIGHT*0.09));
    loginLabel->setGeometry(QRect(WINDOW_WIDTH*0.15,WINDOW_HEIGHT*0.75,WINDOW_WIDTH*0.7,WINDOW_WIDTH*0.7/540*88));
//    settingLabel->setGeometry(QRect(WINDOW_WIDTH*0.92,WINDOW_HEIGHT*0.93,WINDOW_WIDTH*0.06,WINDOW_WIDTH*0.06));
}

/**
 * @brief 按键监听事件
 * @param QKeyEvent
 */
void LoginDialog::keyPressEvent(QKeyEvent *e)
{

    if(e->key() == Qt::Key_Return || e->key () == Qt::Key_Enter)
    {
        loginClick();
    }
}
/**
 * @brief 鼠标按下
 * @param QMouseEvent
 */
void LoginDialog::mousePressEvent(QMouseEvent *e)
{
    if(e->button () == Qt::LeftButton)
    {
        m_Drag = true;
        m_position = e->globalPos () - this->pos ();
        e->accept ();
    }
}

/**
 * @brief 鼠标移动
 * @param QMouseEvent
 */
void LoginDialog::mouseMoveEvent(QMouseEvent *e)
{
    if(m_Drag &&(e->buttons () && Qt::LeftButton))
    {
        move (e->globalPos () - m_position);
        e->accept ();
    }
}

/**
 * @brief 鼠标松开
 * @param QMouseEvent
 */
void LoginDialog::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    m_Drag = false;
}
