#include "settingdialog.h"
#include <QSettings>
#include <QMessageBox>
#include <QKeyEvent>

#define WINDOW_WIDTH this->size().width()
#define WINDOW_HEIGHT this->size().height()

SettingDialog::SettingDialog(QWidget *parent)
{
    Q_UNUSED(parent);

    this->setWindowTitle(QString::fromLocal8Bit("设置"));

    prefix = "http://";

    iniHelper = new INIHelper1();
    // 设置UI
    setupUI();
    // 读取上次存储的信息
    readStoreInfo();
}

SettingDialog::~SettingDialog()
{
    delete iniHelper;
}

/**
 * @brief 设置UI
 */
void SettingDialog::setupUI()
{
    IPLabel = new QLabel(this);
    IPLabel->setText(QString::fromLocal8Bit("IP"));
    IPLabel->setAlignment(Qt::AlignCenter);

    IPEdit = new QLineEdit(this);

    portLabel = new QLabel(this);
    portLabel->setText(QString::fromLocal8Bit("端口"));
    portLabel->setAlignment(Qt::AlignCenter);

    portEdit = new QLineEdit(this);

    httpBox = new QComboBox(this);
    httpBox->addItem("HTTP");
    httpBox->addItem("HTTPS");
    connect(httpBox,SIGNAL(currentIndexChanged(int)),this,SLOT(httpBoxIndexChanged(int)));

    sureLabel = new QPushButton(this);
    sureLabel->setText(QString::fromLocal8Bit("确定"));
    connect(sureLabel,SIGNAL(clicked()),this,SLOT(sureClick()));
}

/**
 * @brief http选择
 * @param 选中的索引
 */
void SettingDialog::httpBoxIndexChanged(int index)
{
    if(index == 0)
    {
        prefix = "http://";
    }
    else if(index == 1)
    {
        prefix = "https://";
    }
}

/**
 * @brief 读取之前存储的IP端口信息
 */
void SettingDialog::readStoreInfo()
{
    QString url = iniHelper->readWebUrl();
    if(url.length()>0)
    {
        QStringList list = url.split(":");
        QString IP = list.at(1);
        if(IP.contains("//"))
        {
            IP = IP.replace("//","");
        }
        QString port = list.at(2);
        IPEdit->setText(IP);
        portEdit->setText(port);
    }
}

/**
 * @brief 确定点击事件
 */
void SettingDialog::sureClick()
{
    if(portEdit->text().length()>0 && IPEdit->text().length()>0)
    {
        iniHelper->writeWebInfo(prefix,IPEdit->text(),portEdit->text());
        QString webSiteURL = QString("%1%2:%3/liveAssistant/index").arg (prefix).arg(IPEdit->text()).arg(portEdit->text());
        iniHelper->writeWebSiteInfo("WebSiteInfo/URL",webSiteURL);
        this->close();
    }
    else
    {
        QMessageBox warnMsgBox(QMessageBox::Critical,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("IP和端口都不能为空"));
        warnMsgBox.setStandardButtons (QMessageBox::Yes|QMessageBox::No);
        warnMsgBox.setButtonText (QMessageBox::Yes,QString::fromLocal8Bit ("确定"));
        warnMsgBox.setButtonText (QMessageBox::No,QString::fromLocal8Bit ("取消"));

        int button = warnMsgBox.exec ();
        if(button == QMessageBox::Yes)
        {
            exit(0);
        }
    }
}

/**
 * @brief 界面布局事件
 * @param QResizeEvent
 */
void SettingDialog::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    IPLabel->setGeometry(QRect(WINDOW_WIDTH*0.1,WINDOW_HEIGHT*0.1,WINDOW_WIDTH*0.1,WINDOW_HEIGHT*0.2));
    IPEdit->setGeometry(QRect(WINDOW_WIDTH*0.3,WINDOW_HEIGHT*0.15,WINDOW_WIDTH*0.6,WINDOW_HEIGHT*0.1));
    portLabel->setGeometry(QRect(WINDOW_WIDTH*0.1,WINDOW_HEIGHT*0.35,WINDOW_WIDTH*0.1,WINDOW_HEIGHT*0.2));
    portEdit->setGeometry(QRect(WINDOW_WIDTH*0.3,WINDOW_HEIGHT*0.4,WINDOW_WIDTH*0.6,WINDOW_HEIGHT*0.1));
    httpBox->setGeometry(QRect(WINDOW_WIDTH*0.1,WINDOW_HEIGHT*0.65,WINDOW_WIDTH*0.8,WINDOW_HEIGHT*0.1));
    sureLabel->setGeometry(QRect(WINDOW_WIDTH*0.7,WINDOW_HEIGHT*0.8,WINDOW_WIDTH*0.2,WINDOW_HEIGHT*0.15));
}

/**
 * @brief 按键监听事件
 * @param QKeyEvent
 */
void SettingDialog::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return)
    {
        sureClick();
    }
}
