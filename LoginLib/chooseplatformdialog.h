#ifndef CHOOSEPLATFORMDIALOG_H
#define CHOOSEPLATFORMDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include "inihelper.h"
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QWebEngineView>
#include "appcommand.h"
#include <QProgressBar>

class ChoosePlatformDialog : public QDialog
{
     Q_OBJECT
public:
    explicit ChoosePlatformDialog(QWidget *parent = 0);
    ~ChoosePlatformDialog();

    void setWebUrl(QString url);
private:
    INIHelper1 *iniHelper;
    QWebEngineView *m_Webview;
    AppCommand1 *command;
    QProgressBar *loadingProgressBar;

    void setupUI();
    void clearCache();

protected:
    void resizeEvent(QResizeEvent *e);

signals:
    void sureClick();

private slots:
    void webSiteInfoReceived(QString info);
    void webViewLoadProgress(int value);
    void closeDialog();
};

#endif // CHOOSEPLATFORMDIALOG_H
