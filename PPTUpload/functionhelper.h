#ifndef FUNCTIONHELPER_H
#define FUNCTIONHELPER_H

#include <QObject>

class FunctionHelper : public QObject
{
    Q_OBJECT
public:
    explicit FunctionHelper(QObject *parent = nullptr);
    static FunctionHelper *sharedInstance();                    // 单实例
    static void Release();                                      // 释放资源
    void startFunction(char* argv[]);                                       // 开始功能

private:
    static FunctionHelper *m_Instance;
signals:

};

#endif // FUNCTIONHELPER_H
