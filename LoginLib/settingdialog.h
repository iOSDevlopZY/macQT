#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "inihelper.h"
#include <QComboBox>

class SettingDialog:public QDialog
{
    Q_OBJECT
public:
     explicit SettingDialog(QWidget *parent = 0);
     ~SettingDialog();
private:
    QLabel *IPLabel;       // IP标签
    QLabel *portLabel;     // 端口标签
    QLineEdit *IPEdit;     // IP输入框
    QLineEdit *portEdit;   // 端口输入框
    QPushButton *sureLabel;// 确定按钮
    INIHelper1 *iniHelper; // INI帮助类
    QComboBox *httpBox;    // http/https选择
    QString prefix;        // http前缀

    void setupUI();        // 设置UI
    void readStoreInfo();  // 读取INI信息

protected:
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);
private slots:
    void httpBoxIndexChanged(int index); // http选择事件
    void sureClick();                    // 确定按钮点击事件
};

#endif // SETTINGDIALOG_H
