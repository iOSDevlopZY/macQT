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
    QString imgPath2 = QCoreApplication::applicationDirPath() + "/labelImage1.png";
    dragLabel = new DragLabel(this);
    dragLabel->setLabelImage(imgPath);
    dragLabel2 = new DragLabel(this);
    dragLabel2->setLabelImage(imgPath2);
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
    dragLabel->setGeometry(0,0,this->size().width()*0.5,this->size().height()*0.5);
    dragLabel2->setGeometry(0,this->size().height()*0.5,this->size().width()*0.5,this->size().height()*0.5);
    dropLabel->setGeometry(this->size().width()*0.5,0,this->size().width()*0.5,this->size().height());
}

