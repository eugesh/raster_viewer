#ifndef KRESTSCENE_H
#define KRESTSCENE_H

#include <QGraphicsScene>
//#include "form_mainwindow_krest.h"

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
     //enum Mode { InsertItem, InsertLine, InsertText, MoveItem};
     //enum numbKrst{movekursor,krestXL,krestYU,krestXR,krestYD};     
     KrestScene(QWidget *parent = 0);
     QPointF cursor_scene_pos() ;
     //QFont font() const
     //     { return myFont; }
     //QColor textColor() const
     //      { return myTextColor; }
     //  QColor itemColor() const
     //     { return myItemColor; }
     //         QColor lineColor() const
     //   { return myLineColor; }
     //void setLineColor(const QColor &color);
     //void setTextColor(const QColor &color);
     //void setItemColor(const QColor &color);
     //void setFont(const QFont &font);

//public slots:
     //void setMode(Mode mode);
     //void setItemType(DiagramItem::DiagramType type);
     //void editorLostFocus(DiagramTextItem *item);


 protected:
     void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
     void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

 signals:;
     void ScaleRubberSignal(QRect rect);
     //void CurrentPixel(QPointF const&Pixel) ;

     //void changeCoordLine(){};
     //void itemInserted(DiagramItem *item);
     //void textInserted(QGraphicsTextItem *item);
     //void itemSelected(QGraphicsItem *item);
 private:
     QPointF startPos,endPos; //начальная/конечноя позиции курсора при зуммировании 
     //bool isItemChange(int type);
     QPointF cursorPos;
     //Krest::KrestType myKrestType;
     //QMenu *myItemMenu;
     //Mode myMode;
     //bool leftButtonDown;
     QPointF startPoint;
     //QGraphicsLineItem *line;
     //QFont myFont;
     //DiagramTextItem *textItem;
     //QColor myTextColor;
     //QColor myItemColor;
     //QColor myLineColor;
};

#endif
