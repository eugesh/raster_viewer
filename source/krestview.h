#ifndef CITYVIEW_H
#define CITYVIEW_H

#include <QGraphicsView>
#include "krest.h"
//#include "form_mainwindow_krest.h"

class KrestView : public QGraphicsView {
public:
	bool cutting;
	//QPushButton *KVpushButtonFit;
	//bool rubberBandIsShown;
	QRect rubberBandRect;
	//enum { Margin = 50 };
	//double scale;//!< коэффициент масштаба
	//! конструктор
	KrestView (QWidget *parent = 0);
	double x0,y0;//!< координаты левого верхнего угла растра в СК xy  
	double c_R,r_R;//!< положение курсора на правом снимке в СК правого снимка	
	int dr,dc;
	double scaled;
public:;
	//void setPushButtonFit (QPushButton *QPB) {KVpushButtonFit=QPB;};
private:;
	int HEIGHT_WINDOW, WIDTH_WINDOW;
	bool is_move;//!< флаг режима перемещения
	QPoint pressed_left_button;
	QCursor prev_curcor;//!< курсор окна
	QCursor null_curcor;
protected:;
	virtual void	mouseMoveEvent (QMouseEvent * event);
	//virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );  
	//void updateRubberBandRegion();
	//! отработка нажатия на кнопку мыши
	//virtual void	mousePressEvent ( QMouseEvent * event );
	//! отработка отпускания на кнопки мыши
	//virtual void	mouseReleaseEvent ( QMouseEvent * event );
	//!отработка сигнала перемещения мыши  
	//virtual void	mouseMoveEvent ( QMouseEvent * event );    
	//! отработка нажатия на кнопки
	//virtual void keyPressEvent ( QKeyEvent * event ) ;  
	//! отработка поворота колесика мыши
	void wheelEvent (QWheelEvent * event);
	//! отработка сигнала перемещения мыши
	//virtual void	mouseMoveEvent ( QMouseEvent * event );   
public slots:;
	//void ScaleFit() {this->fitInView(this->scene()->itemsBoundingRect(),Qt::KeepAspectRatio);} ;

signals:;
	//! сигнал изменения масштаба
	void change_scale (double scale);

	//! сигнал о положении курсора на снимке
	void position_on_shot(double x,double y) {};
};

#endif
