#ifndef CITYVIEW_H
#define CITYVIEW_H

#include <QGraphicsView>
#include "krest.h"

class KrestView : public QGraphicsView {
public:;
	bool cutting;
	QRect rubberBandRect;
	//! Конструктор.
	KrestView (QWidget *parent = 0);
	//!< Координаты левого верхнего угла растра в СК xy.
	double x0, y0;
	//!< Положение курсора на правом снимке в СК правого снимка.
	double c_R, r_R;
	int dr, dc;
	double scaled;

private:;
	int HEIGHT_WINDOW, WIDTH_WINDOW;
	//!< Флаг режима перемещения.
	bool is_move;
	QPoint pressed_left_button;
	//!< Курсор окна.
	QCursor prev_curcor;
	QCursor null_curcor;

protected:;
	virtual void	mouseMoveEvent (QMouseEvent * event);
	//! Отработка поворота колесика мыши.
	void wheelEvent (QWheelEvent * event);

public slots:;

signals:;
	//! Cигнал изменения масштаба.
	void change_scale (double scale);

	//! Cигнал о положении курсора на снимке.
	void position_on_shot(double x,double y) {};
};

#endif
