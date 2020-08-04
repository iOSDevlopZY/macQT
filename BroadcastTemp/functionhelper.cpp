#include "functionhelper.h"
#include "inihelper.h"
#include "networkhelper.h"
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QUuid>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

// 静态变量需要在类体的外面进行初始化
FunctionHelper *FunctionHelper::m_Instance = NULL;

FunctionHelper::FunctionHelper(QObject *parent) : QObject(parent)
{
    IP = "";
    Port = "";
    uploadSrcDir = "";
    timerID = 0;
}

FunctionHelper::~FunctionHelper()
{

}

/**
 * @brief 单实例方法
 * @return
 */
FunctionHelper *FunctionHelper::shareInstance()
{
    if(m_Instance == NULL)
    {
        m_Instance = new FunctionHelper();
        atexit(Release);
    }
    return m_Instance;
}

/**
 * @brief 开始执行参数
 * @param 参数
 */
void FunctionHelper::startFunction(char *argv[])
{
    qDebug()<<QString::fromLocal8Bit("----------- 开始 -----------");
    try {
        // 读取IP和端口信息
        QString filePath = QCoreApplication::applicationDirPath()+"/URL.ini";
        IP = IniHelper::shareInstance()->readIniInfo(filePath,"Info/IP");
        Port = IniHelper::shareInstance()->readIniInfo(filePath,"Info/Port");
        qDebug()<<QString::fromLocal8Bit("----------- 读取IP和端口号 -----------");
        qDebug()<<QString(">>> IP:%1,Port:%2").arg(IP).arg(Port);
        // 解析命令行参数
        char* cmdParam = argv[1];
        QString paramStr(cmdParam);
        qDebug()<<(QString::fromLocal8Bit(">>> 传入命令行参数为:"))<<paramStr;
        QStringList paramList = paramStr.split('#');
        if(paramList.count() > 1)
        {
            // 上传
            if(paramList[0] == "0")
            {
                action = ACTION::UPLOAD;
                uploadSrcDir = paramList[1];
                qDebug()<<QString::fromLocal8Bit(">>> 上传文件夹路径为:")<<uploadSrcDir;

            }
            // 下载
            else
            {
                action = ACTION::DOWNLOAD;
            }
            // 开启定时器
            startActionTimer();
        }
        else
        {
            qWarning()<<QString::fromLocal8Bit("命令行参数不符合要求");
            exit(-1);
        }

    } catch (...) {
        recordResult(QString::fromLocal8Bit("startFunction出现异常"));
        exit(-1);
    }
}

/**
 * @brief 开启定时器
 */
void FunctionHelper::startActionTimer()
{
    timerID = startTimer(100);
}

/**
 * @brief 记录日志
 * @param 内容
 */
void FunctionHelper::recordResult(QString res)
{
    try {
        QString filePath = QCoreApplication::applicationDirPath() + "/result.res";
        QFile resFile(filePath);
        if(!resFile.open(QIODevice::Text|QFile::ReadWrite))
        {
            qWarning()<<QString::fromLocal8Bit("无法创建或者打开result.res文件");
            return ;
        }
        resFile.write(res.toUtf8());
        resFile.flush();
        resFile.close();
    } catch (...) {
        qWarning()<<QString::fromLocal8Bit("recordResult出现异常");
    }
}

/**
 * @brief 上传处理方法
 */
void FunctionHelper::onUpload()
{
    try {
        QString guid = QUuid::createUuid().toString();
        guid = guid.replace("-","");
        guid = guid.replace("{","");
        guid = guid.replace("}","");
        bool ok = false;
        QFileInfoList files = getFiles(uploadSrcDir);
        int sendOK = 0;
        int index = 0;
        qDebug()<<QString::fromLocal8Bit(">>> 上传文件数量:")<<files.count();
        QDate dt(QDate::currentDate());
        QString month = dt.month()<10?QString("0%1").arg(QString::number(dt.month())):QString::number(dt.month());
        QString day = dt.day()<10?QString("0%1").arg(QString::number(dt.day())):QString::number(dt.day());
        QString tmpGuid = QString("%1\\%2\\%3").arg(dt.year()).arg(month).arg(day);
        guid = QString("%1\\%2").arg(tmpGuid).arg(guid);
        // 上传文件
        foreach(QFileInfo info , files)
        {
            index ++;
            QFile uploadFile(info.filePath());
            if(uploadFile.exists())
            {
                if(uploadFile.open(QIODevice::ReadOnly))
                {
                    QByteArray ba = uploadFile.readAll();
                    QString url = QString("http://192.168.2.68:7000/api/FileOp/PostUploadTemplate?"
                                          "templateKey=%1&filename=%2").arg(guid).arg(info.filePath().remove(0, uploadSrcDir.length()));
                    // 每个文件有三次上传机会
                    for(int i = 0;i < 3; i++)
                    {
                        QByteArray res = NetworkHelper::sharedInstance()->uploadFile(url,ba);
                        QJsonParseError error;
                        QJsonDocument doc = QJsonDocument::fromJson(res,&error);
                        if(error.error == QJsonParseError::NoError)
                        {
                            QJsonObject obj = doc.object();
                            if(obj["code"].toString() != "200")
                            {
                                qWarning()<<QString::fromLocal8Bit("上传文件失败：")<<info.fileName();
                            }
                            else
                            {
                                qDebug()<<QString::fromLocal8Bit("上传文件成功：")<<info.fileName();
                                ++sendOK;
                                recordResult(QString("%1,%2,%3").arg(TRANSTAG::UPLOADING).arg(index).arg(files.count()));
                                break;
                            }
                        }
                    }
                    uploadFile.close();
                }
                else
                {
                    qWarning()<<QString(QString::fromLocal8Bit("%1文件无法打开")).arg(info.filePath());
                }
            }
            else {
                qWarning()<<QString(QString::fromLocal8Bit("%1文件不存在")).arg(info.filePath());
            }
        }
        if(sendOK == files.count())
            ok = true;
        if(ok)
        {
            recordResult(QString("%1,%2").arg(TRANSTAG::UPLOAD_OK).arg(guid));
        }
        exit(0);
    } catch (...) {
        qWarning()<<QString::fromLocal8Bit("onUpload出现异常");
    }
}

/**
 * @brief 获取文件夹下所有文件
 * @param 文件夹路径
 */
QFileInfoList FunctionHelper::getFiles(QString dirPath)
{
    QFileInfoList filesList;
    try{
         QDir dir(dirPath);
         QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
         QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

         for(int i = 0; i != folder_list.size(); i++)
         {
              QString name = folder_list.at(i).absoluteFilePath();
              QFileInfoList child_file_list = getFiles(name);
              file_list.append(child_file_list);
         }

        filesList = file_list;
    } catch (...) {
        qWarning()<<QString::fromLocal8Bit("getFiles出现异常");
    }
    return filesList;
}

/**
 * @brief 释放指针
 */
void FunctionHelper::Release()
{
    if (m_Instance != NULL)
    {
        delete m_Instance;
        m_Instance = NULL;
    }
}

/**
 * @brief 定时器事件
 * @param event
 */
void FunctionHelper::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerID)
    {
        killTimer(timerID);
        timerID = 0;
        switch (action) {
            // 执行上传
            case ACTION::UPLOAD:
                qDebug()<<QString::fromLocal8Bit("------- 执行上传 -------");
                onUpload();
                break;
            // 执行下载
            case ACTION::DOWNLOAD:
                qDebug()<<QString::fromLocal8Bit("------- 执行下载 -------");
                break;
        }
    }
}
