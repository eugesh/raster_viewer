#include <QtGui>

#include "krest.h"


Krest::Krest(QGraphicsItem * parent)
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkGreen;
    //myBackgroundColor = Qt::white;
    myBackgroundColor = QColor(255,255,255,0);
    myKrestColor = Qt::darkRed;

    setFlags(ItemIsMovable | ItemIsSelectable);
    setFlag(ItemIgnoresTransformations,true);
    //setFlags(QGraphicsItem::ItemIgnoresTransformations);
}
Krest::~Krest( )
{

}
/*
Node::~Node()
{
    foreach (Link *link, myLinks)
        delete link;
} */

void Krest::setText(const QString &text)
{
    prepareGeometryChange( );
    myText = text;
//    myText = "+";
    update( );
}

QString Krest::text( )
{
    return myText;
}

/*void Krest::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
   if(coord.x()==-1&&coord.y()==-1){
      coord = buttonDownScenePos(Qt::LeftButton);              //*graphicsView->scaled); 
   }
}*/

/*
void Node::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

QColor Node::textColor() const
{
    return myTextColor;
}

void Node::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor Node::outlineColor() const
{
    return myOutlineColor;
}

void Node::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor Node::backgroundColor() const
{
    return myBackgroundColor;
}

void Node::addLink(Link *link)
{
    myLinks.insert(link);
}

void Node::removeLink(Link *link)
{
    myLinks.remove(link);
}
*/
QRectF Krest::boundingRect() const
{
    const int Margin = 1;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}
/*
QPainterPath Node::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect, roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}
*/
void Krest::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget*/ ) 
{
    //painter->save();
      // option->levelOfDetail;
//    painter->setPen(myTextColor);
    QPen pen(myOutlineColor);
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);
    //painter->setBrush(myOutlineColor);
    QRectF rect = outlineRect();

    /*const int Padding = 8;
    QFontMetricsF metrics = qApp->font();
    //QRectF rect = metrics.boundingRect(myText);
    QRectF rect1 = metrics.boundingRect("+");
    painter->drawText(rect1, Qt::AlignCenter, "+"); 
    //QRectF rect = metrics.boundingRect(rect1,Qt::AlignRight,myText);
    rect1.adjust(-Padding, -Padding, +Padding, +Padding);
    //rect.translate(-rect.center());
    rect1.translate(-8,-8);
    //painter->drawRoundRect(rect, roundness(rect.width()),
    //                          roundness(rect.height())); */

    //QRectF rect1 = metrics.boundingRect(myText);

    painter->drawRoundedRect(rect,25,25);   
    //рисуем крестик
    //painter->setBrush(myKrestColor);
    //QLine 

    painter->setPen(myTextColor);
    //painter->setFont(QFont("Times",50,1));//QFont::Normal));
    //painter->drawText(rect, Qt::AlignCenter, "+");
    painter->setFont(QFont("Times",25,1));
                        //painter->drawText(rect,rect.center()+25,rect.center()+25,myText);
    painter->drawText(rect,QTextOption::LeftTab,myText);
    QVector<QPoint> pointPairs;
    pointPairs << QPoint (0,25) << QPoint (0,-25) ;
    painter->drawLines(pointPairs) ;
    pointPairs.clear () ;
    pointPairs << QPoint (25,0) << QPoint (-25,0) ;
    painter->drawLines(pointPairs) ;
    coord = this->pos();  //! сохранение координат
    //std::cout<<coord.x()<<" "<<coord.y()<<std::endl;

 /* int s,sm;
  sm = 4;
 s=2*sm+1;
 QImage marker;
 marker=QImage(s,s,QImage::Format_ARGB32);

 marker.fill(qRgba(255,255,255,0));
 QColor color(255,0,0);

 int k;

 for(k=0;k<s;k++) {
  marker.setPixel(sm,k,color.rgba ());
  marker.setPixel(k,sm,color.rgba ());
 };
  marker.setPixel(sm,sm,qRgba(255,255,255,0)); */
}

void Krest::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),
                           tr("»зменение текста"), tr("¬ведите текст:"),
                           QLineEdit::Normal, myText);
    if (!text.isEmpty())
        setText(text);
}
/*
QVariant Node::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        foreach (Link *link, myLinks)
            link->trackNodes();
    }
    return QGraphicsItem::itemChange(change, value);
}
*/
QRectF Krest::outlineRect() const
{
    const int Padding = 50;
    QFontMetricsF metrics = qApp->font();
    QRectF rect = metrics.boundingRect("+");
    // QRectF rect = metrics.boundingRect(rect1,Qt::AlignRight,myText);
    rect.adjust(-Padding, -Padding, +Padding, +Padding);
    // rect.translate(-rect.center());
    rect.translate(-3.2,4.65);
    return rect;    
    // рисуем крестик
    // painter->setBrush(myKrestColor);
    // QLine vline()
    // QRectF rect = metrics.boundingRect(myText);
}
/*
int Node::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / int(size);
} */

