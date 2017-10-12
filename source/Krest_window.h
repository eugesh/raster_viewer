#ifndef __KREST_WINDOW_H__
#define __KREST_WINDOW_H__

#include <QWidget>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QGraphicsView>
#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QInputDialog>
#include <iostream>
#include <QVector3D>
#include <stdio.h>
#include <iostream>
#include "RubberRect.h"
#include "form_mainwindow_krest.h"
#include "../graphic_item_image/graphic_item_image.h"
#include "krest.h"
#include "krestview.h"
#include "Krest_scene.h"
#include "cthread.h"

class QAction;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsView;
class KrestView;

double MathRound(double num, int precision);

//! Класс, описывающий окно отображения кадра.
class KrestWindow : public QMainWindow, public Ui::MainWindow {
	  Q_OBJECT
private:
	KrestScene *scene;
	QGraphicsItem_Image *QGI_Image;
	bool is_move; //!< флаг режима перемещени¤
	double c, r; //!< положение курсора
	QAction *pushButtonFit;
	CThread *pThread;
	QPointF startPos,endPos; //начальная/конечноя позиции курсора при зуммировании
	Krest KrestXL, KrestXR, KrestYU, KrestYD;
public:;
    double PixelSize; // размер пикселя в мм
    double FrameSizeMm; // размер рамки в мм
    int FrameSizePx; // размер рамки в пикселях
    bool useSizeMm; // использовать задание рамки в мм
    QInputDialog *QIDSFSMm; // немодальные диалоги задания размера рамки в мм
    QInputDialog *QIDSFSPx; // в пикселях
    QPointF Center;
    RubberRect *RR;
    bool cutting;
    bool iscutting;
    int cut_count;
    QString nameIn; // image path+name
    QString nameOut; // image name
    QString pathOut; // image path
    QString pathPoint; // points path
  	//! Конструктор
    KrestWindow();
    ~KrestWindow();
    QPointF XL, XR, YU, YD;
    // enum numbKrst{movekursor, krestXL, krestYU, krestXR, krestYD};
    Krest*selectedKrest();
    void calculCoordKrest();
    // bool openImage(const QString &fileName);
    // bool saveImage(const QString &fileName, const char *fileFormat);
    // void setImage(const QImage &image);

private slots:;
   void openImage ();
   void openPoints();
   void savePoints();
   void savePointsAs(); 

   void addKrestRX( QPointF F = QPointF(-1,-1) );  //! добавление крестов
   void addKrestLX( QPointF F = QPointF(-1,-1) );
   void addKrestUY( QPointF F = QPointF(-1,-1) );
   void addKrestDY( QPointF F = QPointF(-1,-1) );
   void changeCoord(double x,double y);
   void changeFrameSize(int);//задание размера рамки в пикселх
   void changeFrameSize(double); 
   void cursorCross();
   void cursorHand ();
   void cursorRubber();   
   void cutRect();
   void endCut ();
   // void ctrlZ_Cut();
   // void ctrlZ_All_Cut();
   // void updateCoord(QPointF,QPointF);
   void del();   //! удаление
   // void show_position_on_shot(double c,double r); //! отображение координат курсора
   void updateActions(); //! обновление действий
   void pickAllKrest( );   //catch all krest
   //!слот запуска рассчета
   // void pressed_button_run ();
   // void pressed_button_run2();
   void run_process();
   void ShowPixel   (QPointF const&Pixel) ;
   void SetPixelSize();  //задание размера пикселя в мм
   void SetFrameSizeMm();//задание размера рамки в мм
   void SetFrameSizePx();//задание размера рамки в пикселях
   void SaveTrAs();
   void SaveTr();
   void ScaleFit(){graphicsView->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);} ;
   void ScaleRubberFit(QRect);
   void StopProcess();
private:;
    void createActions();
    void createMenus();
    void setupKrest( Krest *krest, QPointF F );

    // void setupKrest(Krest *krest);
protected:
    // void updateRubberBandRegion();
    // void wheelEvent(QWheelEvent *event);
   	//! отработка нажати¤ на кнопку мыши
    virtual void	mousePressEvent( QGraphicsSceneMouseEvent * event );
    // virtual void	mousePressEvent ( QMouseEvent * event );
  	//! отработка отпускани¤ на кнопки мыши
    virtual void	mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
    // virtual void	mouseReleaseEvent( QMouseEvent * event );
   	//! отработка сигнала перемещени¤ мыши
    // virtual void	mouseMoveEvent( QMouseEvent * event );
   	// virtual void	mouseMoveEvent( QGraphicsSceneMouseEvent * event );
   	//! отработка нажати¤ на кнопки
    // virtual void keyPressEvent( QKeyEvent * event );
signals:;
    //! сигнал о положении курсора на снимке
    // void position_on_shot(double c,double r);
};


/*class Block_transform {//: public QVector3D {
public:
  Block_transform( QPoint, QPoint, QPoint, QPoint, int BlockSizeX_OUT, int BlockSizeY_OUT );
//private:
  QVector3D ro_block[4];
  QVector3D e1_block, e2_block, b_block, rc_block;
  int  r_block,c_block;
  double c0b_OUT,r0b_OUT;//, r0_block,c0_block;
  double e11_block,e12_block,e22_block,e1b_block,e2b_block;
  double D_block,D1_block,t1_block;      
};*/

#endif
