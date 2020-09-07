#include "inihelper.h"
#include <QSettings>
#include <QDebug>
#include <QCoreApplication>

// 静态变量需要在类体的外面进行初始化
IniHelper *IniHelper::m_Instance = NULL;

IniHelper::IniHelper(QObject *parent) : QObject(parent)
{

}

IniHelper::~IniHelper()
{

}

/**
 * @brief 单实例方法
 * @return
 */
IniHelper *IniHelper::shareInstance()
{
    if(m_Instance == NULL)
    {
        m_Instance = new IniHelper();
        atexit(Release);
    }
    return m_Instance;
}

/**
 * @brief 读取INI文件键值信息
 * @param INI文件路径
 * @param 键值名称
 */
QString IniHelper::readIniInfo(QString file, QString key)
{
    QSettings *setting = new QSettings(file,QSettings::IniFormat);
    QString value = setting->value(key).toString();
    delete  setting;
    return value;
}

/**
 * @brief 读取登录用户名
 * @param key
 * @return
 */
QString IniHelper::readLoginInfo(QString key)
{
    QString res = "";

    QString iniPath = QCoreApplication::applicationDirPath() +"/MedcareBroadcastLoginInfo.ini";

    QSettings *IniSettings = new QSettings(iniPath, QSettings::IniFormat);

    res = IniSettings->value(key).toString();

    delete IniSettings;

    return res;
}

/**
 * @brief 释放指针
 */
void IniHelper::Release()
{
    if (m_Instance != NULL)
    {
        delete m_Instance;
        m_Instance = NULL;
    }
}
