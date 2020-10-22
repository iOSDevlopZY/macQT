#include "droplabel.h"
#include <QDebug>

DropLabel::DropLabel(QWidget *parent) : QLabel(parent)
{
    // 设置接收拖拽
    setAcceptDrops(true);
}

/**
 * @brief 放下事件
 * @param e
 */
void DropLabel::dropEvent(QDropEvent *e)
{
    qDebug()<<"dropEvent";
    e->accept();
}

/**
 * @brief 拖拽进入事件
 * @param event
 */
void DropLabel::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("LabelImage"))
    {
          event->setDropAction(Qt::MoveAction);
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
        event->setDropAction(Qt::MoveAction); //b.移动操作
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
