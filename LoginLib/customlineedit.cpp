#include "customlineedit.h"
#include <QPainter>

CustomLineEdit::CustomLineEdit(QWidget *parent):QWidget(parent)
{
    this->setStyleSheet ("QWidget{border-radius:5px}");
    setupUI();
}

/**
 * @brief 设置UI
 */
void CustomLineEdit::setupUI()
{

    QHBoxLayout *layout = new QHBoxLayout();

    logoLabel = new QLabel();
    logoLabel->setScaledContents (true);
    logoLabel->setMaximumWidth (25);
    logoLabel->setMaximumHeight (25);
    logoLabel->setStyleSheet ("background:transparent;");
    layout->addWidget (logoLabel,1);

    entryEdit = new QLineEdit();
    entryEdit->setStyleSheet ("background:transparent;border-width:0;border-style:outset");
    layout->addWidget (entryEdit,10);
    this->setLayout (layout);
}

void CustomLineEdit::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    QPen pen;
    pen.setColor (Qt::gray);
    pen.setWidth (2);
    painter.setPen (pen);

    painter.drawRect (this->rect());
    painter.end ();
}
