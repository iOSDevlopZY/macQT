#include "droplabel.h"
#include <QDebug>

DropLabel::DropLabel(QWidget *parent) : QLabel(parent)
{
    // 设置接收拖拽
    setAcceptDrops(true);
    // 设置居中
    setAlignment(Qt::AlignCenter);
}

/**
 * @brief 放下事件
 * @param e
 */
void DropLabel::dropEvent(QDropEvent *e)
{
    if(e->mimeData()->hasFormat("LabelImage"))
    {
            this->clear();
            QByteArray itemData = e->mimeData()->data("LabelImage");
            QDataStream dataStream(&itemData,QIODevice::ReadOnly);
            QPixmap pixmap;
            dataStream>>pixmap;
            this->setPixmap(pixmap);
            e->accept();
    }
    else
    {
            e->ignore();
    }
}

/**
 * @brief 拖拽进入事件
 * @param event
 */
void DropLabel::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("LabelImage"))
    {
          event->accept();
    }
    else
    {
          event->ignore();
    }
}

/**
 * @brief 拖动移动事件
 * @param event
 */
void DropLabel::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat("LabelImage"))
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

/**
 * @brief 尺寸大小事件
 * @param e
 */
void DropLabel::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    if(this->pixmap())
    {
        QPixmap map = this->pixmap()->scaled(this->size(), Qt::KeepAspectRatio);
        this->setPixmap(map);
    }
}
