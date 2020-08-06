#include "functionhelper.h"
#include "inihelper.h"

FunctionHelper *FunctionHelper::m_Instance = NULL;

FunctionHelper::FunctionHelper(QObject *parent) : QObject(parent)
{

}

/**
 * @brief 单实例
 * @return
 */
FunctionHelper *FunctionHelper::sharedInstance()
{
    if(m_Instance == NULL)
    {
        m_Instance = new FunctionHelper();
        atexit(Release);
    }
    return m_Instance;
}

/**
 * @brief 释放资源
 */
void FunctionHelper::Release()
{
    if(m_Instance)
    {
        delete  m_Instance;
        m_Instance = NULL;
    }
}

void FunctionHelper::startFunction(char* argv[])
{

}
