#ifndef KRESTSCENE_H
#define KRESTSCENE_H

#include <QGraphicsScene>

 class QGraphicsSceneMouseEvent;
 class QMenu;
 class QPointF;
 class QGraphicsLineItem;
 class QFont;
 class QGraphicsTextItem;
 //class QColor;

class KrestScene : public QGraphicsScene
{
     Q_OBJECT
 public:
     KrestScene(QWidget *parent = 0);
     QPointF cursor_scene_pos() ;

 protected:
     void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
     void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

 signals:;
     void ScaleRubberSignal(QRect rect);

 private:
     QPointF startPos,endPos; //начальная/конечноя позиции курсора при зуммировании
     QPointF cursorPos;
     QPointF startPoint;
};

#endif
