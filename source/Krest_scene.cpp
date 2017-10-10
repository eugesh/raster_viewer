#include <QtGui>
#include "Krest_scene.h"
#include <iostream>

KrestScene::KrestScene( QWidget *parent)
     : QGraphicsScene(parent)
{
   cursorPos = QPointF(0,0);
   //myItemMenu = itemMenu;
   //myMode = MoveItem;
   //myItemType = DiagramItem::Step;
   //line = 0;
   //textItem = 0;
   //myItemColor = Qt::white;
   //myTextColor = Qt::black;
   //myLineColor = Qt::black;
}

QPointF KrestScene::cursor_scene_pos() {
   //cursorPos = QCursor::pos();
   return cursorPos;
}

void KrestScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
   cursorPos = mouseEvent->scenePos();
  //emit CurrentPixel(cursorPos) ;
  //emit changeCoordLine();
  /* QString str1,str2;

   str1=QObject::tr("%1").arg(int(cursor_scene_pos().x()));
   str1=QObject::tr("%1").arg(int(cursor_scene_pos().y()));

   lineEdit_X->insert(str1);

   lineEdit_Y->insert(str2); */
   QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void KrestScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
   if(event->button()==Qt::LeftButton) {
      startPos=event->scenePos();
      //std::cout<<"Start Pos"<<startPos.x()<<" "<<startPos.y()<<std::endl;
   }
   QGraphicsScene::mousePressEvent(event);
}
void KrestScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
   if(event->button()==Qt::LeftButton) {
      endPos=event->scenePos();
      //std::cout<<"End Pos"<<endPos.x()<<" "<<endPos.y()<<std::endl;      
      QRect RectRBD(startPos.toPoint(),endPos.toPoint());
      emit ScaleRubberSignal(RectRBD.normalized ());
   }
   QGraphicsScene::mouseReleaseEvent(event);
}
