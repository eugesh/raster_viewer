#include <QtGui>

#include "krest.h"


Krest::Krest(QGraphicsItem * parent)
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkGreen;
    myBackgroundColor = QColor(255,255,255,0);
    myKrestColor = Qt::darkRed;

    setFlags(ItemIsMovable | ItemIsSelectable);
    setFlag(ItemIgnoresTransformations,true);
}

Krest::~Krest( )
{

}

void Krest::setText(const QString &text)
{
    prepareGeometryChange( );
    myText = text;
    update( );
}

QString Krest::text( )
{
    return myText;
}

QRectF Krest::boundingRect() const
{
    const int Margin = 1;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

void Krest::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget*/ ) 
{
    QPen pen(myOutlineColor);
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);
    QRectF rect = outlineRect();

    painter->drawRoundedRect(rect,25,25);   
    painter->setPen(myTextColor);
    painter->setFont(QFont("Times",25,1));
    painter->drawText(rect,QTextOption::LeftTab,myText);
    QVector<QPoint> pointPairs;
    pointPairs << QPoint (0,25) << QPoint (0,-25) ;
    painter->drawLines(pointPairs) ;
    pointPairs.clear () ;
    pointPairs << QPoint (25,0) << QPoint (-25,0) ;
    painter->drawLines(pointPairs) ;
    //! Cохранение координат
    coord = this->pos();
}

void Krest::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),
                           tr("»зменение текста"), tr("Введите текст:"),
                           QLineEdit::Normal, myText);
    if (!text.isEmpty())
        setText(text);
}

QRectF Krest::outlineRect() const
{
    const int Padding = 50;
    QFontMetricsF metrics = qApp->font();
    QRectF rect = metrics.boundingRect("+");
    rect.adjust(-Padding, -Padding, +Padding, +Padding);
    rect.translate(-3.2,4.65);
    return rect;    
    // рисуем крестик
    // painter->setBrush(myKrestColor);
    // QLine vline()
    // QRectF rect = metrics.boundingRect(myText);
}

/*void Krest::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
   if(coord.x()==-1&&coord.y()==-1){
      coord = buttonDownScenePos(Qt::LeftButton);              //*graphicsView->scaled);
   }
}*/

/* QVariant Node::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        foreach (Link *link, myLinks)
            link->trackNodes();
    }
    return QGraphicsItem::itemChange(change, value);
}*/
