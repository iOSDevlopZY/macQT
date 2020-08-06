#ifndef FUNCTIONHELPER_H
#define FUNCTIONHELPER_H

#include <QObject>

enum INFO{
    NOT_EXIST_FILE = 100,
    UPLOAD_FAILED,
    UPLOADING,
    UPLOAD_OK,
    DOWNLOAD_FAILED,
    DOWNLOAD_OK
};

class FunctionHelper : public QObject
{
    Q_OBJECT
public:
    explicit FunctionHelper(QObject *parent = nullptr);
    static FunctionHelper *sharedInstance();                    // 单实例
    static void Release();                                      // 释放资源
    void startFunction(char* argv[]);                           // 开始功能
    void recordResult(QString res);                             // 记录日志

private:
    static FunctionHelper *m_Instance;
    QString IP;                                                 // IP
    QString Port;                                               // 端口号
    QString pptPath;                                            // PPT文件
    QString pptKey;                                             // 下载PPT文件时的标识

    void createOKFIN();                                         // 创建ok.fin文件
signals:

};

#endif // FUNCTIONHELPER_H
