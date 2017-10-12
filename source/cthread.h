#ifndef __CTHREAD_H__
#define __CTHREAD_H__
#include <QtGui>
#include <QThread>
#define MAX_SIZE_BLOCK 4096
//#include "transformation.h"
//#include "form_mainwindow_krest.h"
//! класс, описывающий нить расчетов
/*!
ќписание нити
*/
double roundD(double x);
class settings {
 protected:;
   QString name_in ;
   QString name_out;
   QSize q_out_size;
   QVector<QPointF> qvqp_x; //-x+y+x-y
 public:
   //settings(QString i,QString o, QVector<QPoint> x) {name_in=i;name_out=o;qvqp_x=x;};
   void set_in_name (QString n) {name_in =n;};
   void set_out_name(QString n) {name_out=n;};
   void set_point   (QVector<QPointF> x) {qvqp_x=x;};
   void set_size_out(QSize s) {q_out_size=s;};
   settings( ) ;
};

class CThread : public QThread , public settings {
	Q_OBJECT
public: ;
  QProgressBar *progressbar;
public:;
	//! конструктор
	CThread( QObject *parent = NULL ) ;
  virtual ~CThread() {;} ;

protected: ;
  virtual void run();
  // virtual void run1();
signals:;
	 //! посылка сигнала с количеством обработанных фрагментов изображения
	 void number_worked_file(int nf);
};

#endif
