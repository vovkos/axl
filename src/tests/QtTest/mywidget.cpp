#include "pch.h"
#include "mylogrepresentor.h"
#include "moc_mywidget.cpp"

//.............................................................................

MyWidget::MyWidget(QWidget* parent) :
    QLabel(parent),
    effect_(Plain)
{ 
}
 
void MyWidget::drawTextEffect(QPainter *painter, 
                                 QPoint offset)
{
    Q_ASSERT(painter != 0); 
 
    // Draw shadow.
    painter->setPen(QPen(Qt::black));
    painter->drawText(rect().translated(offset),
                      alignment(), text());
    // Draw text.
    painter->setPen(QPen(Qt::gray));
    painter->drawText(rect(), alignment(), text());
}
 
void MyWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setFont(font());
    if (effect_ == Plain)
        QLabel::paintEvent(event);
    else if (effect_ == Sunken) 
        // Shadow above the text.
        drawTextEffect(&painter, QPoint(0, -1));
    else if (effect_ == Raised) 
        // Shadow below the text.
        drawTextEffect(&painter, QPoint(0, 1));
}

//.............................................................................
