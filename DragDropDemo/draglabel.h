#ifndef DRAGLABEL_H
#define DRAGLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

class DragLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DragLabel(QWidget *parent = 0);
    void setLabelImage(QString imgPath);             // 设置图片

private:

protected:
    void    mousePressEvent(QMouseEvent *e);
    void    dropEvent(QDropEvent *e);
    void    resizeEvent(QResizeEvent *e);
};

#endif // DRAGLABEL_H
