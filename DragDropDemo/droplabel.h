#ifndef DROPLABEL_H
#define DROPLABEL_H

#include <QObject>
#include <QLabel>
#include <QDropEvent>
#include <QMimeData>

class DropLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DropLabel(QWidget *parent = 0);

protected:
    void    dropEvent(QDropEvent *e);
    void    dragEnterEvent(QDragEnterEvent *event);
    void    dragMoveEvent(QDragMoveEvent *event);
signals:

};

#endif // DROPLABEL_H
