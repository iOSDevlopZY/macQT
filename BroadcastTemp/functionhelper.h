#ifndef FUNCTIONHELPER_H
#define FUNCTIONHELPER_H

#include <QObject>
#include <QFileInfoList>

enum ACTION
{
     UPLOAD = 0,
     DOWNLOAD,
     UNKNOWN
};
enum TRANSTAG
{
     FAILED = -1,
     UPLOAD_FAILED = 101,  // PPTCONVERT.UPLOAD_FAILED,
     UPLOADING = 102,      // PPTCONVERT.UPLOADING,
     UPLOAD_OK = 103,      // PPTCONVERT.UPLOAD_OK,
     DOWNLOAD_FAILED = 104,// PPTCONVERT.DOWNLOAD_FAILED ,
     DOWNLOAD_OK = 105     // PPTCONVERT.DOWNLOAD_OK
};

class FunctionHelper : public QObject
{
    Q_OBJECT
public:
    explicit FunctionHelper(QObject *parent = nullptr);
    ~FunctionHelper();
    static FunctionHelper *shareInstance();              // 单实例
    void startFunction(char* argv[]);                    // 开始执行函数操作

private:
    static FunctionHelper *m_Instance;
    QString IP;                                          // API IP
    QString Port;                                        // API端口号
    ACTION action;                                       // 当前模式
    QString uploadSrcDir;                                // 上传文件夹路径
    QString downloadKey;                                 // 下载时KEY
    QString userName;                                    // 登录用户名
    int timerID;                                         // 定时器

    void startActionTimer();                             // 开启定时器
    void recordResult(QString res);                      // 记录错误原因
    void onUpload();                                     // 上传处理方法
    void onDownload();                                   // 下载处理方法
    QFileInfoList getFiles(QString dirPath);             // 获取文件夹下所有文件
    void createFullDir(QString path);                    // 创建下载文件夹
    bool downLoadFile(QString url, QString dst);         // 下载文件
    void getOMSUrlInfo();                                // 获取OMS接口地址
    static void Release();                               // 释放指针
    bool isOfficalEnviorment();                          // 判断是否为正式环境
    void readUserInfo();                                 // 读取用户信息

protected:
    void timerEvent(QTimerEvent *event) override;
signals:

};

#endif // FUNCTIONHELPER_H
