#ifndef INIHELPER_H
#define INIHELPER_H

#include <QObject>
#include <QSettings>

class INIHelper1
{
public:
    INIHelper1();
    void writeAccountInfo(QString key,QString value);          // 写入账户信息
    void writeWebInfo(QString prefix,QString IP,QString port); // 写入配置文件
    void writeLoginInfo(QString key,QString value);            // 写入登录信息
    QString readLoginInfo(QString key);                        // 读取登录信息
    bool checkIniFileExists();                                 // 检查配置文件是否存在
    QString readWebUrl();                                      // 读取接口地址

    void writeWebSiteInfo(QString key,QString value);          // 写入加载的网址
private:

};

#endif // INIHELPER_H
