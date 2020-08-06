#include "functionhelper.h"
#include "inihelper.h"
#include "networkhelper.h"
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

FunctionHelper *FunctionHelper::m_Instance = NULL;

FunctionHelper::FunctionHelper(QObject *parent) : QObject(parent)
{
    IP = "";
    Port = "";
    pptPath = "";
    pptKey = "";
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
    qDebug()<<QString::fromLocal8Bit("----------- 开始 -----------");
    // 创建ok.fin文件
    createOKFIN();

    try {
        // 读取IP和端口信息
        QString filePath = QCoreApplication::applicationDirPath()+"/URL.ini";
        IP = IniHelper::shareInstance(  )->readIniInfo(filePath,"Info/IP");
        Port = IniHelper::shareInstance()->readIniInfo(filePath,"Info/Port");
        qDebug()<<QString::fromLocal8Bit("----------- 读取IP和端口号 -----------");
        qDebug()<<QString(">>> IP:%1,Port:%2").arg(IP).arg(Port);

        // 获取PPT路径
        pptPath = argv[1];
        QFile file(pptPath);
        if (pptPath.length() < 1 || !file.exists())
        {
            file.close();
            recordResult(QString("%1").arg(INFO::NOT_EXIST_FILE));
            exit(-1);
        }
        // 标记为上传中
        recordResult(QString("%1").arg(INFO::UPLOADING));
        file.close();

        QString errorStr = "";
        // 上传尝试三次
        for(int j = 0; j < 3; j++)
        {
            QFile uploadFile(pptPath);
            if(!uploadFile.open(QIODevice::ReadOnly))
            {
                qWarning()<<QString::fromLocal8Bit("打开PPT文件读取流失败！");
                uploadFile.close();
                continue;
            }
            QByteArray fileData = uploadFile.readAll();
            uploadFile.close();
            qDebug()<<QString::fromLocal8Bit("------- 成功读取本地文件 -------");
            QFileInfo fi(pptPath);
            QString fileName = fi.fileName();

            // 正式上传
            QString url = QString("http://%1:%2/api/FileOp/PostUploadPPTFile?fileName=%3")
                    .arg(IP).arg(Port).arg(fileName);
            QByteArray res = NetworkHelper::sharedInstance()->uploadFile(url,fileData);
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(res,&error);
            if(error.error == QJsonParseError::NoError)
            {
                QJsonObject obj = doc.object();
                if(obj["code"].toInt() != 200)
                {
                    errorStr = obj["error"].toString();
                    continue;
                }
                else
                {
                    pptKey = obj["fileKey"].toString();
                    qDebug()<<QString::fromLocal8Bit("上传成功：")<<pptKey;
                    break;
                }
            }
        }
        if(pptKey.length() == 0)
        {
            qWarning()<<QString::fromLocal8Bit("上传失败：")<<errorStr;
            recordResult(QString("%1,%2").arg(INFO::UPLOAD_FAILED).arg(errorStr));
            exit(-1);
        }
        recordResult(QString("%1").arg(INFO::UPLOAD_OK));
    }
    catch(...)
    {
        qWarning()<<QString::fromLocal8Bit("Main函数出现了异常");
        exit(-1);
    }
}

/**
 * @brief 创建ok.fin文件
 */
void FunctionHelper::createOKFIN()
{
#ifdef Q_OS_MACOS
    QString finFilePath = QCoreApplication::applicationDirPath() + "/ok.fin";
#else
    QString finFilePath = QCoreApplication::applicationDirPath() + "\\ok.fin";
#endif
    QFile file(finFilePath);
    if(!file.exists())
    {
        if(!file.open(QIODevice::WriteOnly))
        {
            qWarning()<<QString::fromLocal8Bit("创建OK.fin文件失败");
        }
    }
    file.close();
}

/**
 * @brief 记录日志
 * @param 内容
 */
void FunctionHelper::recordResult(QString res)
{
    try {
#ifdef Q_OS_MACOS
        QString filePath = QCoreApplication::applicationDirPath() + "/result.res";
#else
        QString filePath = QCoreApplication::applicationDirPath() + "\\result.res";
#endif
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
