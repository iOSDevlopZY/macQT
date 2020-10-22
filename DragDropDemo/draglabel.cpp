#include "draglabel.h"
#include <QPainter>
#include <QDebug>

DragLabel::DragLabel(QWidget *parent):QLabel(parent)
{
    // 打开拖拽功能
    setAcceptDrops(true);
    // 设置背景为黑色
    setStyleSheet("background:black;");
}

/**
 * @brief 设置现实的图片
 * @param 图片路径
 */
void DragLabel::setLabelImage(QString imgPath)
{
    QPixmap pixmap(imgPath);
    labelMap = pixmap;
}

/**
 * @brief 鼠标移动事件
 */
void DragLabel::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    QPixmap pixmap = *this->pixmap();
    pixmap = pixmap.scaled(this->size(), Qt::KeepAspectRatio);

    //2 自定义Mime类型数据，存储图片数据和位置信息
    QByteArray itemData;
    QDataStream dataStream(&itemData,QIODevice::WriteOnly);
    dataStream<<pixmap;

    //3 将数据字节数组存入MimeData中，这里相当于setData作用类似<key,value>
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("LabelImage",itemData);

    //4 为了移动数据必须创建Drag类对象，然后为其添加QMimeData数据
    //  QDrag对象会在拖拽完成后被Qt自动回收
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(QPoint(drag->pixmap().width()*0.5,
                            drag->pixmap().height()*0.5));

    //5 在移动图片过程中为原来图片添加了一层阴影
    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(),QColor(127,127,127,127));
    painter.end();
    this->setPixmap(tempPixmap);

    //6 执行拖动操作，默认设置支持复制操作
    if(drag->exec(Qt::CopyAction|Qt::MoveAction,Qt::CopyAction) != Qt::MoveAction)
    {
        // 去掉阴影
        this->setPixmap(pixmap);
    }
    else
    {
        this->setPixmap(pixmap);
    }
}

/**
 * @brief 尺寸大小事件
 * @param e
 */
void DragLabel::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    QPixmap map = labelMap.scaled(this->size(), Qt::KeepAspectRatio);
    this->setPixmap(map);
}
