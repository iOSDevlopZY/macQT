#ifndef INIHELPER_H
#define INIHELPER_H

#include <QObject>

class IniHelper : public QObject
{
    Q_OBJECT
public:
    explicit IniHelper(QObject *parent = nullptr);
    ~IniHelper();
    static IniHelper *shareInstance();               // 单实例方法
    QString readIniInfo(QString file,QString key);   // 读取INI文件键值信息
    QString readLoginInfo(QString key);              // 读取INI用户名信息

private:
    static IniHelper* m_Instance;                    // 单实例
    static void Release();                           // 释放
signals:

};

#endif // INIHELPER_H
