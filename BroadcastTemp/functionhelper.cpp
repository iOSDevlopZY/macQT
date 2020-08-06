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
#include <QJsonArray>
#include <QTimer>

// 静态变量需要在类体的外面进行初始化
FunctionHelper *FunctionHelper::m_Instance = NULL;

FunctionHelper::FunctionHelper(QObject *parent) : QObject(parent)
{
    IP = "";
    Port = "";
    uploadSrcDir = "";
    downloadKey = "";
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
#ifdef Q_OS_MACOS
                downloadKey = paramList[1].replace("\\","/");
#else
                downloadKey = paramList[1];
#endif
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
            if(info.fileName() == ".DS_Store" || !info.fileName().contains("."))
                continue ;
            index ++;
            QFile uploadFile(info.filePath());
            if(uploadFile.exists())
            {
                if(uploadFile.open(QIODevice::ReadOnly))
                {
                    QByteArray ba = uploadFile.readAll();
                    QString url = QString("http://%1:%2/api/FileOp/PostUploadTemplate?"
                                          "templateKey=%3&filename=%4").arg(IP).arg(Port).arg(guid).arg(info.filePath().remove(0, uploadSrcDir.length()));
                    // 每个文件有三次上传机会
                    for(int i = 0;i < 3; i++)
                    {
                        QByteArray res = NetworkHelper::sharedInstance()->uploadFile(url,ba);
                        QJsonParseError error;
                        QJsonDocument doc = QJsonDocument::fromJson(res,&error);
                        if(error.error == QJsonParseError::NoError)
                        {
                            QJsonObject obj = doc.object();
                            if(obj["code"].toInt() != 200)
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

    } catch (...) {
        qWarning()<<QString::fromLocal8Bit("onUpload出现异常");
    }
     exit(0);
}

/**
 * @brief 下载处理方法
 */
void FunctionHelper::onDownload()
{
    try
    {
        QString url = QString("http://%1:%2/api/FileOp/PostGetTemplateFiles?"
                                            "templateKey=%3").arg(IP).arg(Port).arg(downloadKey);
        QByteArray res = NetworkHelper::sharedInstance()->postRequest(url);

        if(res.length() == 0)
        {
            recordResult(QString("%1,%2").arg(TRANSTAG::DOWNLOAD_FAILED).arg(QString::fromLocal8Bit("获取文件列表失败")));
            exit(-1);
        }
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(res,&error);
        if(error.error == QJsonParseError::NoError)
        {
            QJsonObject obj = doc.object();
            if(obj["code"].toInt() != 200)
            {
                recordResult(QString("%1,%2").arg(TRANSTAG::DOWNLOAD_FAILED).arg(obj["error"].toString()));
                exit(-1);
            }
            else
            {
                QJsonArray ja = obj["result"].toArray();
#ifdef Q_OS_MACOS
                QString downloadPath = QCoreApplication::applicationDirPath() + "/download/"+ downloadKey;
#else
                QString downloadPath = QCoreApplication::applicationDirPath() + "\\download\\"+ downloadKey;
#endif
                createFullDir(downloadPath);
                for(int i = 0 ;i < ja.count(); i++)
                {
                    QJsonObject obj = ja.at(i).toObject();
                    QString filePath = obj["path"].toString();
                    QString fileName = obj["file"].toString();
                    QString url = QString("http://%1:%2/%3").arg(IP).arg(Port).arg(filePath);
#ifdef Q_OS_MACOS
                    QString dst = QString("%1/%2").arg(downloadPath).arg(fileName.replace("\\","/"));

#else
                    QString dst = QString("%1\\%2").arg(downloadPath).arg(fileName);
#endif
                    QFileInfo fileInfo(dst);
                    // 创建文件夹
                    createFullDir(fileInfo.dir().path());

                    bool res = false;
                    // 下载文件，每个文件尝试三次
                    for(int i = 0;i < 3;i++)
                    {
                        res = downLoadFile(url,dst);
                        if(res)
                            break;
                    }
                    // 下载失败
                    if(!res)
                    {
                        recordResult(QString("%1,%2").arg(TRANSTAG::DOWNLOAD_FAILED).arg(QString(QString::fromLocal8Bit("%1下载失败！")).arg(url)));
                        qWarning()<<url<<QString::fromLocal8Bit("下载失败，关闭主程序");
                        exit(-1);
                    }
                }
                recordResult(QString("%1,%2").arg(TRANSTAG::DOWNLOAD_OK).arg(downloadPath));

            }
        }
        else
        {
            recordResult(QString("%1,%2").arg(TRANSTAG::DOWNLOAD_FAILED).arg(QString::fromLocal8Bit("下载JSON解析失败！")));
            qWarning()<<QString::fromLocal8Bit("下载JSON解析失败！");
        }
    } catch (...) {
        qWarning()<<QString::fromLocal8Bit("onDownload出现异常");
        recordResult(QString("%1,%2").arg(TRANSTAG::DOWNLOAD_FAILED).arg(QString::fromLocal8Bit("onDownload出现异常")));
    }
    exit(0);
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
 * @brief 创建下载文件夹
 * @param 文件夹
 */
void FunctionHelper::createFullDir(QString path)
{
    QFile downloadFile(path);
    if(!downloadFile.exists())
    {
#ifdef Q_OS_MACOS
        QString dirPath = path;
        QStringList pathList = dirPath.split('/');
        if(pathList.length() > 1)
        {
            QString path = pathList[0];
            for (int i = 1;i < pathList.count();i++)
            {
                path.append(QString("/%1").arg(pathList.at(i)));
                QDir dir(path);

                if(!dir.exists())
                {
                    dir.mkpath(path);
                }
            }
        }
#else
        QString dirPath = path.left(path.lastIndexOf('\\'));
        QStringList pathList = dirPath.split('\\');
        if(pathList.length() > 1)
        {
            QString path = pathList[0];
            for (int i = 1;i < pathList.count();i++)
            {
                path.append(QString("\\%1").arg(pathList.at(i)));
                QDir dir(path);
                if(!dir.exists())
                {
                    dir.mkpath(path);
                }
            }
        }
#endif
    }
    downloadFile.close();
}

/**
 * @brief 下载文件
 * @param url
 * @param 目标路径
 */
bool FunctionHelper::downLoadFile(QString url, QString dst)
{
    // 超时时间30S
    int timeout = 30000;

    QFile f(dst);

    // 创建下载文件
    if (!f.open(QIODevice::WriteOnly)) {
        qWarning()<<QString::fromLocal8Bit("创建或打开下载文件失败：")<<dst;
        f.close();
        return false;
    }
    QNetworkAccessManager m;
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    QNetworkReply *reply = m.get(req);
    QEventLoop loop;
    QTimer t;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::readyRead,
                        [=, &f, &t](){
           f.write(reply->readAll());
           f.flush();
           if (t.isActive()) {
               t.start(timeout);
           }
    });
    if (timeout > 0) {
           QObject::connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
           t.start(timeout);
    }
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        f.close();
        qWarning()<<QString::fromLocal8Bit("下载文件网络失败：")<<QString::fromLocal8Bit(reply->errorString().toUtf8());
        return false;
    }
   f.close();
   reply->deleteLater();
   return true;
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
                onDownload();
                break;
        }
    }
}
