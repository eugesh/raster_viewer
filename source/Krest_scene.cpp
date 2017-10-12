#include <QtGui>
#include "Krest_scene.h"
#include <iostream>

KrestScene::KrestScene( QWidget *parent)
     : QGraphicsScene(parent)
{
   cursorPos = QPointF(0,0);
}

QPointF KrestScene::cursor_scene_pos() {
   //cursorPos = QCursor::pos();
   return cursorPos;
}

void KrestScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
   cursorPos = mouseEvent->scenePos();

   QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void KrestScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
   if(event->button()==Qt::LeftButton) {
      startPos=event->scenePos();
   }
   QGraphicsScene::mousePressEvent(event);
}
void KrestScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
   if(event->button()==Qt::LeftButton) {
      endPos=event->scenePos();
      QRect RectRBD(startPos.toPoint(),endPos.toPoint());
      emit ScaleRubberSignal(RectRBD.normalized ());
   }
   QGraphicsScene::mouseReleaseEvent(event);
}
