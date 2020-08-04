#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>

class CustomLineEdit : public QWidget
{
public:
    explicit CustomLineEdit(QWidget *parent = 0);
    QLineEdit *entryEdit;                        // 输入框
    QLabel *logoLabel;                           // logo
private:
    void setupUI();                              // 设置UI
protected:
    void paintEvent (QPaintEvent *e);
};

#endif // CUSTOMLINEEDIT_H
