#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Drag & Drop Demo");
    QString imgPath = QCoreApplication::applicationDirPath() + "/labelImage.png";
    dragLabel = new DragLabel(this);
    dragLabel->setLabelImage(imgPath);
    dropLabel = new DropLabel(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief 尺寸大小事件
 * @param e
 */
void MainWindow::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    dragLabel->setGeometry(0,0,this->size().width()*0.5,this->size().height());
    dropLabel->setGeometry(this->size().width()*0.5,0,this->size().width()*0.5,this->size().height());
}

