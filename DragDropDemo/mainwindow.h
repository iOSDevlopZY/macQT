#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "draglabel.h"
#include "droplabel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DragLabel *dragLabel;
    DropLabel *dropLabel;

protected:
    void resizeEvent(QResizeEvent *e);
};
#endif // MAINWINDOW_H
