#include "clicklabel.h"
#include <QDebug>

ClickLabel::ClickLabel(QWidget *parent) : QLabel(parent)
{

}

/**
 * @brief 鼠标按下事件
 * @param QMouseEvent
 */
void ClickLabel::mouseReleaseEvent(QMouseEvent *e)
{
       if(e->button()==Qt::LeftButton)
       {
           emit Clicked();
       }
       QLabel::mouseReleaseEvent(e);
}

/**
 * @brief 鼠标双击事件
 * @param QMouseEvent
 */
void ClickLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit Clicked();
    QLabel::mouseDoubleClickEvent(event);
}

