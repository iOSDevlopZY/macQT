#include "inihelper.h"
#include <QFile>
#include <QCoreApplication>

INIHelper1::INIHelper1()
{

}

/**
 * @brief 写入账户信息
 * @param 账户字段
 * @param 账户字段值
 */
void INIHelper1::writeAccountInfo(QString key, QString value)
{
    QString iniPath = QCoreApplication::applicationDirPath() +"/BroadcastAccountInfo.ini";

    QSettings *IniSettings = new QSettings(iniPath, QSettings::IniFormat);

    IniSettings->setIniCodec("utf-8");

    IniSettings->setValue(key,QVariant(value));

    delete IniSettings;
}

/**
 * @brief 写入IP和端口
 * @param IP
 * @param port
 */
void INIHelper1::writeWebInfo(QString prefix,QString IP, QString port)
{
    QString iniPath = QCoreApplication::applicationDirPath() +"/MedcareBroadcastWebURL.ini";

    QSettings *IniSettings = new QSettings(iniPath, QSettings::IniFormat);

    IniSettings->setValue("MedcareBroadcastWebSite/URL",QVariant(QString("%1%2:%3").arg(prefix).arg(IP).arg(port)));

    delete IniSettings;
}

/**
 * @brief 写入登录用户名
 * @param 登录用户
 * @param 登录用户名
 */
void INIHelper1::writeLoginInfo(QString key, QString value)
{
    QString iniPath = QCoreApplication::applicationDirPath() +"/MedcareBroadcastLoginInfo.ini";

    QSettings *IniSettings = new QSettings(iniPath, QSettings::IniFormat);

    IniSettings->setValue(key,QVariant(value));

    delete IniSettings;
}

/**
 * @brief 读取登录信息
 * @param 登录值
 * @return
 */
QString INIHelper1::readLoginInfo(QString key)
{
    QString res = "";

    QString iniPath = QCoreApplication::applicationDirPath() +"/MedcareBroadcastLoginInfo.ini";

    QSettings *IniSettings = new QSettings(iniPath, QSettings::IniFormat);

    res = IniSettings->value(key).toString();

    delete IniSettings;

    return res;
}

/**
 * @brief 检测配置文件是否存在
 * @return
 */
bool INIHelper1::checkIniFileExists()
{
     QString iniPath = QCoreApplication::applicationDirPath() +"/MedcareBroadcastWebURL.ini";

     QFile file(iniPath);

     bool res = file.exists();

     file.close();

     return res;
}

/**
 * @brief 读取接口地址
 * @return 接口地址
 */
QString INIHelper1::readWebUrl()
{
    QString url = "";

    if(checkIniFileExists())
    {
        QString iniPath = QCoreApplication::applicationDirPath() +"/MedcareBroadcastWebURL.ini";

        QSettings *IniSettings = new QSettings(iniPath, QSettings::IniFormat);

        url = IniSettings->value("MedcareBroadcastWebSite/URL").toString();

        delete IniSettings;
    }

    return url;
}

/**
 * @brief 写入加载的网址
 * @param key
 * @param value
 */
void INIHelper1::writeWebSiteInfo(QString key, QString value)
{
    QString iniPath = QCoreApplication::applicationDirPath() +"/BroadcastWebSiteInfo.ini";

    QSettings *IniSettings = new QSettings(iniPath, QSettings::IniFormat);

    IniSettings->setValue(key,QVariant(value));

    delete IniSettings;
}
