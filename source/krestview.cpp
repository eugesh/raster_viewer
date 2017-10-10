#include <QtGui>
#include <cmath>
#include <iostream>

#include "krestview.h"
//! конструктор
KrestView::KrestView(QWidget *parent)
    : QGraphicsView(parent)
{
    //QCursor(Qt::CrossCursor);
    //setCursor(Qt::CrossCursor);
    setDragMode(QGraphicsView::ScrollHandDrag);  
    dr=dc=0;
    x0=0;
    y0=height();
    is_move=false;
    HEIGHT_WINDOW=height();
    WIDTH_WINDOW=width();
    scaled=1.0;
    cutting = false;
    //rubberBandIsShown = false;
    setMouseTracking(true);
    //connect(KVpushButtonFit,SIGNAL(clicked()),SLOT(ScaleFit()));
    //viewport()->setCursor(Qt::CrossCursor);
    //c_L=r_L=0;
} 
//! отработка поворота колесика мыши
void KrestView::wheelEvent(QWheelEvent *event)
{
  if (!is_move) {
  QPoint  pos;
  double x,y;
  double r,c;
  // не в режиме перемещения
  int delta;

  delta=event->delta();

  pos=event->pos();
  x=pos.x();
  y=pos.y();

  c=(x-x0)/scaled;
  r=(y0-y)/scaled;

  // if (delta>0) scaled=scaled*2;
  // else scaled=scaled/2;

  // x0=x-scaled*c;
  // y0=y+scaled*r;

  // посылка сигнала о изменении масштаба
  // emit change_scale(scale);
  
    double numDegrees = event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    //double factor = std::pow(1.125, numSteps);
    scaled = std::pow(1.125, numSteps);
    //this->scale(factor, factor);
    this->scale(scaled, scaled);
    x0=x-scaled*c;
    y0=y+scaled*r;
  }
}

/*void KrestView::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{   
   if(cutting) {
      QRect rect(Margin, Margin,
                 width() - 2 * Margin, height() - 2 * Margin);
  
      if (event->button() == Qt::LeftButton) {
          if (rect.contains(event->pos().toPoint ())) {
              rubberBandIsShown = true;
              rubberBandRect.setTopLeft(event->pos().toPoint ());
              rubberBandRect.setBottomRight(event->pos().toPoint ());
              updateRubberBandRegion();
             // setCursor(Qt::CrossCursor);
          }
      }
  }
}
*/
void KrestView::mouseMoveEvent ( QMouseEvent * event )
{
   /* if (rubberBandIsShown) {
        updateRubberBandRegion();
        rubberBandRect.setBottomRight(event->pos().toPoint ());
        updateRubberBandRegion();
    }*/
    double x,y;
    x=event->pos().x();
    y=event->pos().y();    
    emit position_on_shot(x,y);
    QGraphicsView::mouseMoveEvent(event);
}

/*void KrestView::mousePressEvent ( QMouseEvent * event )
{   
   if(cutting) {
      QRect rect(Margin, Margin,
                 width() - 2 * Margin, height() - 2 * Margin);
  
      if (event->button() == Qt::LeftButton) {
          if (rect.contains(event->pos())) {
              rubberBandIsShown = true;
              rubberBandRect.setTopLeft(event->pos());
              rubberBandRect.setBottomRight(event->pos());
              updateRubberBandRegion();
              setCursor(Qt::CrossCursor);
          }
      }
  }
}

void KrestView::updateRubberBandRegion()
{
    QRect rect = rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    printf("\n%d %d %d",rect.left(), rect.top(), rect.width());
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
    
}

void KrestView::mouseMoveEvent ( QMouseEvent * event )
{
    if (rubberBandIsShown) {
        updateRubberBandRegion();
        rubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
    }
}

void KrestView::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && rubberBandIsShown) {
        rubberBandIsShown = false;
        cutting = false;
        updateRubberBandRegion();
        unsetCursor();
        setDragMode(QGraphicsView::ScrollHandDrag);
      }
 /*       QRect rect = rubberBandRect.normalized();
        if (rect.width() < 4 || rect.height() < 4)
            return;
        rect.translate(-Margin, -Margin);

        PlotSettings prevSettings = zoomStack[curZoom];
        PlotSettings settings;
        double dx = prevSettings.spanX() / (width() - 2 * Margin);
        double dy = prevSettings.spanY() / (height() - 2 * Margin);
        settings.minX = prevSettings.minX + dx * rect.left();
        settings.maxX = prevSettings.minX + dx * rect.right();
        settings.minY = prevSettings.maxY - dy * rect.bottom();
        settings.maxY = prevSettings.maxY - dy * rect.top();
        settings.adjust();

        zoomStack.resize(curZoom + 1);
        zoomStack.append(settings);
        zoomIn();
    }
}

//! отработка сигнала перемещения мыши
/*void KrestView::mouseMoveEvent ( QMouseEvent * event )
{
 /*if (is_move) {
 }
 else {
 QString str;
 //c = event->globalX();
// r = event->globalY();
 int c = event->x();
 int r = event->y();
// emit position_on_shot((double)c,(double)r);
  }
// std::cout<<c<<" "<<r<<endl;
// emit position_on_shot((double)c,(double)r);

// str=QObject::tr("%1").arg(int(c));

// lineEdit_X->setText(str);

// str=QObject::tr("%1").arg(int(r));
 
// lineEdit_Y->setText(str);*/

// Qt::KeyboardModifiers modif;

// modif=event->modifiers();
 
/* if (is_move) {
  this->setDragMode(QGraphicsView::ScrollHandDrag); 
  std::cout<<"event!";
        update();
        updateGeometry();  
 }
/* else {
  double x,y;

  x=event->pos().x();
  y=height()-1-event->pos().y();
  

   c_R=(x-x0)/scaled-dc;
   r_R=(y0-y)/scaled-dr;

//   emit position_on_shot(c_R,r_R);
//   is_draw_only_curcor=true;
   // перерисовка окна
  
  };*/

// }; 
//} 

//! отработка нажатия на кнопку мыши
/*void KrestView::mousePressEvent ( QMouseEvent * event )
{
//    setDragMode(ScrollHandDrag); 
    //setMouseTracking(true);
// Qt::KeyboardModifiers modif;

// modif=event->modifiers();

// if (modif==Qt::NoModifier) {
  switch(event->button()) {
   case Qt::LeftButton:
    {
     // установка режима перемещения
//     std::cout<<"drag!";
     is_move=true;
     // запоминание положения точки где было нажатие
      pressed_left_button=event->pos();
     // установка курсора
//     prev_curcor=this->cursor ();
     this->setCursor(Qt::ClosedHandCursor);
     setDragMode(QGraphicsView::ScrollHandDrag);      
     std::cout<<"drag!";
        update();
        updateGeometry();     
    };
    break;
   case Qt::RightButton:
    {
    };
    break;   
  };
 //}
// else {
 // prev_curcor=this->cursor ();
 // this->setCursor(Qt::BlankCursor); 
 };
//}

//! отработка отпускания на кнопки мыши
void KrestView::mouseReleaseEvent ( QMouseEvent * event )
{
// Qt::KeyboardModifiers modif;

 //modif=event->modifiers();

 if (is_move) {
  std::cout<<"drop!";
  if (event->button()==Qt::LeftButton) {
   QPoint delta;
   //сброс режима перемещения
   is_move=false;
   // получение координаты точки
   delta=event->pos()-pressed_left_button;
   // пересчет области просмотра
   //printf("dx=%d dy=%d\n",delta.x(),delta.y());
   x0+=delta.x();
   y0+=-delta.y();
   // сброс курсора
   this->setCursor(prev_curcor);
   // перерисовка окна
        update();
        updateGeometry();   

  };
 }
/* else {
  //if (modif==Qt::NoModifier) {
   switch(event->button()) {
    case Qt::LeftButton:
     {
     };
     break;
    case Qt::RightButton:
     {
      // вызов контекстного меню
     };
     break;   
   };
  //};  
 };*/

/* if (Qt::BlankCursor==cursor ().shape ()) {
  // сброс курсора
  this->setCursor(prev_curcor);  
 };  
}

/*void KrestView::keyPressEvent ( QKeyEvent * event ) 
{
 //QString name_keys;

 //name_keys=event->text ();
 
 int key;

 key=event->key();

 

 printf("key=%d\n",key);

 switch(key) {
  case Qt::Key_W:;
   r_L--;
   break;
  case Qt::Key_S:;
   r_L++;
   break;
  case Qt::Key_A:;
   c_L--;
   break;
  case Qt::Key_D:;
   c_L++;
   break;
   
  case Qt::Key_8:;
   r_R--;
   break;
  case Qt::Key_2:;
   r_R++;
   break;
  case Qt::Key_4:;
   c_R--;
   break;
  case Qt::Key_6:;
   c_R++;
   break;
 };
 emit position_on_shot(1, c_L,r_L);
 emit position_on_shot(2, c_R,r_R);
// is_draw_only_curcor=true;
 // перерисовка окна
 
 //printf("name_keys=%s\n",name_keys.toLocal8Bit().data());

 //event->accept ();
 //QGLWidget::keyPressEvent(event);
} */

