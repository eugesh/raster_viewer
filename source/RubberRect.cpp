#include <QtGui>

#include "RubberRect.h"

RubberRect::RubberRect(QRectF RectF,QRectF maxRectF,QPointF Ctr,int th,QGraphicsItem * parent): QGraphicsItem(parent)
{
    Center=Ctr;
    max_imgRect = maxRectF;
  //  sizeF_max = RectF.size();
    thickness = th;
    bchanges = false;
    cutting = false;
    bmove = false;
    corner = 0;
    coord1 = RectF.topLeft();
    coord2 = RectF.topRight();
    coord3 = RectF.bottomRight();
    coord4 = RectF.bottomLeft();
    sizeF = QRectF(coord1,coord3).size();
    myOutlineColor = Qt::darkGreen;
    setFlags(ItemIsMovable | ItemIsSelectable);
    //puts("NO error!");
//    setFlag(ItemIgnoresTransformations,true);
}

void RubberRect::setCenter(QPointF C) {
   Center=C;
   qreal sizeX = sizeF.width ();
   qreal sizeY = sizeF.height();   
   change_coord(QPointF(Center.x()-sizeX/2,Center.y()-sizeY/2),1);
   change_coord(QPointF(Center.x()+sizeX/2,Center.y()-sizeY/2),2);
   change_coord(QPointF(Center.x()+sizeX/2,Center.y()+sizeY/2),3);
   change_coord(QPointF(Center.x()-sizeX/2,Center.y()+sizeY/2),4);
}

void RubberRect::SetSize(int size) {
   change_coord(QPointF(Center.x()-size/2,Center.y()-size/2),1);
   change_coord(QPointF(Center.x()+size/2,Center.y()-size/2),2);
   change_coord(QPointF(Center.x()+size/2,Center.y()+size/2),3);
   change_coord(QPointF(Center.x()-size/2,Center.y()+size/2),4);
   sizeF=QSizeF(size,size);
}

void RubberRect::setP1(QPointF newp1){
     if ( newp1 != coord1 ) {
//      if (max_imgRect.contains( coord1)) {
         prepareGeometryChange();
         coord1 = newp1;
         coord4.setX(coord1.x());
         coord2.setY(coord1.y());
         sizeF = QRectF(coord1,coord3).size();
         update();
      }
 //    }
}

void RubberRect::setP2(QPointF newp2){
     if ( newp2 != coord2 ) {
         prepareGeometryChange();
         coord2 = newp2;
         coord3.setX(coord2.x());
         coord1.setY(coord2.y());
         sizeF = QRectF(coord1,coord3).size();
         update();
     }
}

void RubberRect::setP3(QPointF newp3){
     if ( newp3 != coord3 ) {
         prepareGeometryChange();
         coord3 = newp3;
         coord2.setX(coord3.x());
         coord4.setY(coord3.y());
         sizeF = QRectF(coord1,coord3).size();
         update();
     }
}

void RubberRect::setP4(QPointF newp4){
     if ( newp4 != coord4 ) {
         prepareGeometryChange();
         coord4 = newp4;
         coord1.setX(coord4.x());
         coord3.setY(coord4.y());
         sizeF = QRectF(coord1,coord3).size();
         update();
     }
}

int RubberRect::changed_corner( QPointF pos ) {
   corner = 0;
   QRectF rect1(coord1,QPointF(coord1.x()+H_CORNER*thickness,coord1.y()+H_CORNER*thickness));   
   QRectF rect2(QPointF(coord2.x()-H_CORNER*thickness,coord2.y()),QPointF(coord2.x(),coord2.y()+H_CORNER*thickness));
   QRectF rect3(QPointF(coord3.x()-H_CORNER*thickness,coord3.y()-H_CORNER*thickness),coord3);
   QRectF rect4(QPointF(coord4.x(),coord4.y()-H_CORNER*thickness),QPointF(coord4.x()+H_CORNER*thickness,coord4.y()));
   if(boundingRect().contains(pos));
    corner = 5;
   if(rect1.contains(pos))
    corner = 1; //topleft
   if(rect2.contains(pos))
    corner = 2;   //topright
   if(rect3.contains(pos))
    corner = 3;   //bottomright
   if(rect4.contains(pos))
    corner = 4;   //bottomleft

   return corner;
}

bool RubberRect::change_coord(QPointF point,int corner) {
   bool is_changed = 0;
   
   switch(corner) {
    case 1:
      if (max_imgRect.topLeft().x()>point.x()) //отслеживается выход 
        point.setX(max_imgRect.topLeft().x()); //за границу растра
      if (max_imgRect.topLeft().y()>point.y()) 
        point.setY(max_imgRect.topLeft().y());      
        setP1(point);    
      break;
    case 2:
      if (max_imgRect.topRight().x()<point.x()) 
        point.setX(max_imgRect.topRight().x());
      if (max_imgRect.topRight().y()>point.y()) 
        point.setY(max_imgRect.topRight().y());       
      setP2(point);      
      break;
    case 3:
      if (max_imgRect.bottomRight().x()<point.x()) 
        point.setX(max_imgRect.bottomRight().x());
      if (max_imgRect.bottomRight().y()<point.y()) 
        point.setY(max_imgRect.bottomRight().y());       
      setP3(point);      
      break;      
    case 4:
      if (max_imgRect.bottomLeft().x()>point.x()) 
        point.setX(max_imgRect.bottomLeft().x());
      if (max_imgRect.bottomLeft().y()<point.y()) 
        point.setY(max_imgRect.bottomLeft().y());      
      setP4(point);      
      break;     
    }
   return is_changed;
}

QRectF RubberRect::boundingRect() const
{
    return QRectF(coord1, coord3);
}

void RubberRect::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{   
      //  printf("good11!");
   
   if(cutting) {
     corner = changed_corner(event->pos());
    // printf(" %.1f %.1f %d",event->pos().x(),event->pos().y(),corner);
     // if (event->button() == Qt::LeftButton&&corner) {
       if(corner<5&&corner>0){
             //   printf("good22!");
        change_coord(event->pos(),corner);
        bchanges = true;
        }
       if(corner==5) {
        //origin = event->pos();
        bmove=true;
        }
      }
}

void RubberRect::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
  // static int corner;
   // if (rubberBandIsShown) {
   //  corner = changed_corner(event->pos());
      if(bchanges){
        change_coord(event->pos(),corner);
//printf("\nsizeF H%.1f W%.1f P1 X%.1f Y%.1f P2 %.1f %.1f P3 %.1f %.1f P4 %.1f %.1f ",sizeF.height(),
 // sizeF.width(),coord1.x(),coord1.y(),coord2.x(),coord2.y()
           //    ,coord3.x(),coord3.y(),coord4.x(),coord4.y());       
      //  printf("good33!");
      }
      if(bmove) {
    //    moveBy (event->pos().x()-origin.x(),event->pos().y()-origin.y());
      }
    //    updateRubberBandRegion();
     //   rubberBandRect.setBottomRight(event->pos());
     //   updateRubberBandRegion();
     
  //  }

  //   if (rubberBand)
   //      rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
    // QAbstractItemView::mouseMoveEvent(event);
}

void RubberRect::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) {
  bchanges = false;
  bmove = false;
}

void RubberRect::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget*/ ) 
{
   // painter->save();
      // option->levelOfDetail;
//    painter->setPen(myTextColor);
    QPen pen(myOutlineColor);
  //  if (option->state & QStyle::State_Selected) {
     //   pen.setStyle(Qt::DotLine);
     //   pen.setWidth(2);
  //  }
    pen.setWidth(2*thickness);
    painter->setPen(pen);
    QRectF rect(coord1,coord3); //= outlineRect();
    painter->drawRect(rect); 
    pen.setWidth(thickness);
    painter->setPen(pen);
    QRectF rect1(coord1,QPointF(coord1.x()+H_CORNER*thickness,coord1.y()+H_CORNER*thickness));
    painter->drawRect(rect1); 
    
    QRectF rect2(QPointF(coord2.x()-H_CORNER*thickness,coord2.y()),QPointF(coord2.x(),coord2.y()+H_CORNER*thickness));
    painter->drawRect(rect2); 
    
    QRectF rect3(QPointF(coord3.x()-H_CORNER*thickness,coord3.y()-H_CORNER*thickness),coord3);
    painter->drawRect(rect3); 
    
    QRectF rect4(QPointF(coord4.x(),coord4.y()-H_CORNER*thickness),QPointF(coord4.x()+H_CORNER*thickness,coord4.y()));
    painter->drawRect(rect4);  
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
/*QRectF Krest::outlineRect() const
{
    const int Padding = 50;
    QFontMetricsF metrics = qApp->font();
    QRectF rect = metrics.boundingRect("+");
//    QRectF rect = metrics.boundingRect(rect1,Qt::AlignRight,myText);
    rect.adjust(-Padding, -Padding, +Padding, +Padding);
//    rect.translate(-rect.center());
    rect.translate(-3.2,4.65);
    return rect;    
      //рисуем крестик
  //  painter->setBrush(myKrestColor);
 //   QLine vline()
//    QRectF rect = metrics.boundingRect(myText);
}
*/
