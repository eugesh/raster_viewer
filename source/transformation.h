#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__
#include <QtGui>
#include <QPoint>
#include <QVector>
#include <QString>
#include <QSize>
#include <QProgressBar>
#define MAX_SIZE_BLOCK 4096

double roundD(double x);

class  abstractProgress {
 public:
   //progress = [0,1]
   //stop = {0,1} = {break,continue}
   virtual void progressEvent(float progress, QString const&message)=0;
   virtual bool get_stop()=0;
   virtual void set_stop(bool st)=0;
   virtual ~abstractProgress( ){;};
};

class CprogressBar:public abstractProgress {
   private:
     float progress;
     bool stop;
   public:
   CprogressBar() { progress = 0; stop=false; };
   void progressEvent(float x, QString const& msg) {
      progress=x;
      //std::cout<<qPrintable(msg)<<", progress: "<<progress<<std::endl;
   };
   void set_stop(bool st) {stop=st;};
   bool get_stop( ) {return stop ;};
   float get_progress() {return progress;};
   ~CprogressBar ( ) {progress = 0;};
};

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

class transformation : public settings {
	Q_OBJECT
public:;
  QProgressBar *progressbar;
  transformation() {progressbar=NULL;};
  virtual ~transformation() {;};
protected:;
  int run_process(abstractProgress*);
signals:;
	 //! посылка сигнала с количеством обработанных фрагментов изображения
	 void number_worked_file(int nf);
};

#endif