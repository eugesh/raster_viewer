#include <QtGui>
#include <cmath>
#include <math.h>
#include <QCursor>
#include <QPointF>
#include <QString>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QFileDialog>
#include <QMessageBox>
#include <stdlib.h>
#include <QGraphicsSceneMouseEvent>
#include "Krest_window.h"
#include "Krest_scene.h"
#include <QImage>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "moiGdal.h"
#define DEBUG 0
#define MAX_SIZE_BLOCK 4096


KrestWindow::KrestWindow() {
	setupUi(this);
	KrestXL.setText(tr("X-"));
	KrestXR.setText(tr("X+"));
	KrestYU.setText(tr("Y+"));
	KrestYD.setText(tr("Y-"));
	pThread = NULL;
	RR = NULL;
	pushButtonFit=NULL;

	scene = new KrestScene;
	pushButtonFit = new QAction(this);
	pushButtonFit->setText(QObject::tr("Вписать в окно"));
	graphicsView->setScene(scene);
	// graphicsView->setPushButtonFit(pushButtonFit);
	graphicsView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
	graphicsView->setContextMenuPolicy(Qt::ActionsContextMenu);
	graphicsView->setDragMode(QGraphicsView::ScrollHandDrag); // Перетаскивание окна рукой
	QGI_Image = new QGraphicsItem_Image;
	scene->addItem(QGI_Image);
	QColor blackColor(0, 0, 0);
	QPen blackPen(blackColor);
	QBrush blackBrash(blackColor);
	QGI_Image->setZValue(-1000.0);
	createActions();

	connect(scene, SIGNAL(selectionChanged()),
	        this, SLOT(updateActions()));
	// connect(graphicsView, SIGNAL(position_on_shot(double,double)),
	//        this, SLOT(changeCoord(double,double)));
	// connect(scene,SIGNAL(CurrentPixel(QPointF const&)),
	//       this,SLOT(ShowPixel(QPointF const&)));

	FrameSizeMm=0.0;
	FrameSizePx=0;
	PixelSize=1.0;
	cutting = false;
	cut_count = 0;
	iscutting=false;
	QIDSFSMm=new QInputDialog(this);
	QIDSFSPx=new QInputDialog(this);    
	QIDSFSMm->setComboBoxEditable(true);
	QIDSFSPx->setComboBoxEditable(true);    
	QIDSFSMm->setDoubleMaximum(2147483647);
	QIDSFSPx->setIntMaximum(2147483647);    
	QIDSFSMm->setDoubleMinimum(0);
	QIDSFSPx->setIntMinimum(0); 
	QIDSFSMm->setCancelButtonText(tr("Выход"));
	QIDSFSPx->setCancelButtonText(tr("Выход"));
	QIDSFSMm->setOkButtonText(tr("Ввод"));
	QIDSFSPx->setOkButtonText(tr("Ввод"));
	QIDSFSMm->setLabelText(tr("Введите размер в миллиметрах"));
	QIDSFSPx->setLabelText(tr("Введите размер в пикселях"));
	connect(QIDSFSMm,SIGNAL(doubleValueChanged(double)),this,SLOT(changeFrameSize(double)));
	connect(QIDSFSPx,SIGNAL(intValueChanged(int)),this,SLOT(changeFrameSize(int)));
	connect(pushButtonFit,SIGNAL(triggered()),this,SLOT(ScaleFit()));
	connect(scene,SIGNAL(ScaleRubberSignal(QRect)),this,SLOT(ScaleRubberFit(QRect)));
	updateActions();
};


KrestWindow::~KrestWindow() {
   if(RR != NULL)
       delete RR;
   if(pushButtonFit != NULL)
       delete pushButtonFit;
}


void KrestWindow::changeFrameSize(int size) {
   FrameSizePx = size;
   if(cutting) {
      RR->SetSize(FrameSizePx);      
   }
   updateActions( );
}


void KrestWindow::changeFrameSize(double size) {
   FrameSizeMm = size;
   FrameSizePx = (int)ceil(FrameSizeMm / PixelSize);
   if(cutting) {
      RR->SetSize(FrameSizePx);      
   }
   updateActions();
}


void 
KrestWindow::ShowPixel(QPointF const&Pixel) {
    // QString Text = "[ " + QString::number(int(Pixel.x())) + ", "
    //                   + QString::number(int(Pixel.y())) + " ]" ;
    // label_pixel->setText(Text) ;
} ;


void KrestWindow::addKrestRX(QPointF F) {
    Krest * krest;
    QList<QGraphicsItem *> items = scene->items( );
    foreach (QGraphicsItem *item, items) {
      if(krest = dynamic_cast<Krest *>(item)) {
          if(krest->text()=="+X") {           // правый  крестик "+Х"
             scene->removeItem(krest);
          }  
      }
    }
    krest = new Krest(QGI_Image);
    krest->setText(tr("%1").arg("+X"));
    setupKrest(krest,F);
}


void KrestWindow::addKrestLX(QPointF F)
{
    Krest * krest;
    QList<QGraphicsItem *> items = scene->items( );
    foreach (QGraphicsItem *item, items) {
      if(krest = dynamic_cast<Krest *>(item)) {
          if(krest->text()=="-X") {           // правый  крестик "+Х"
             scene->removeItem(krest);
          }  
      }
    }
    krest = new Krest(QGI_Image);
    krest->setText(tr("%1").arg("-X"));
    setupKrest(krest,F);
}


void KrestWindow::addKrestUY(QPointF F)
{
    Krest * krest;
    QList<QGraphicsItem *> items = scene->items( );
    foreach (QGraphicsItem *item, items) {
      if(krest = dynamic_cast<Krest *>(item)) {
          if(krest->text()=="+Y") {           // правый  крестик "+Х"
             scene->removeItem(krest);
          }  
      }
    }
    krest = new Krest(QGI_Image);
    krest->setText(tr("%1").arg("+Y"));
    setupKrest(krest,F);
}


void KrestWindow::addKrestDY(QPointF F)
{
    Krest * krest;
    QList<QGraphicsItem *> items = scene->items( );
    foreach (QGraphicsItem *item, items) {
      if(krest = dynamic_cast<Krest *>(item)) {
          if(krest->text()=="-Y") {           // правый  крестик "+Х"
             scene->removeItem(krest);
          }  
      }
    }
    krest = new Krest(QGI_Image);
    krest->setText(tr("%1").arg("-Y"));
    setupKrest(krest,F);
}


void KrestWindow::pickAllKrest( ) {
    QList<QGraphicsItem *> allItems = scene->items ();
    QGraphicsItem *item;
       foreach(item,allItems)
          item->setSelected(true);
}


void KrestWindow::setupKrest(Krest *krest,QPointF F) {
    QPointF curs;
    QPointF newpos;
      curs = scene->cursor_scene_pos();
    if(F.x()==-1&&F.y()==-1){ //если не заданы координаты (-1,-1) - по умолчанию
       krest->setPos(curs);        
    }
    else
       krest->setPos(F); 
   
    krest->coord = krest->scenePos ();  //! сохранение координат
    scene->addItem(krest);
    krest->setZValue(0.0);
    scene->clearSelection();
    krest->setSelected(true);
}


void KrestWindow::updateActions() {
    bool hasSelection = !scene->selectedItems().isEmpty();
    bool isNode = (selectedKrest() != 0);

    action_Delete->setEnabled(hasSelection);
    actionCross->setEnabled(!hasSelection);
    actionHand->setEnabled(!hasSelection);
    {//! Add action to context menu.
       foreach (QAction *action, graphicsView->actions())
           graphicsView->removeAction(action);
   
       foreach (QAction *action, menuTools->actions()) {  
           if (action->isEnabled())
               graphicsView->addAction(action);
       }
       
       if (pushButtonFit->isEnabled())
          graphicsView->addAction(pushButtonFit);    
    }  
    calculCoordKrest();
    Krest * krest;
    QList<QGraphicsItem *> items = scene->items( );
    int count=0;
    foreach (QGraphicsItem *item, items) {
      if(krest = dynamic_cast<Krest *>(item)) {
          if(krest->text()=="+X") {           // правый  крестик "+Х"
             count++;
          }
          if(krest->text()=="-X") {           // левый   крестик "-Х"
             count++;
          }
          if(krest->text()=="+Y") {           // верхний крестик "+Y"
             count++;
          } 
          if(krest->text()=="-Y") {           // нижний  крестик "-Y"
             count++;
          }      
      }
        if(count==4) { //если все кресты установлены, пересчитать положение центра рамки
           QVector3D ro[4], roIN[4];
           QVector3D e1, e2, b, rc;
           QVector3D la1, la2, la;
           double r0, c0;
         
           ro[0] = QVector3D(YD.x(), YD.y(), 0);   //!!координаты крестов->QVector3D
           ro[1] = QVector3D(YU.x(), YU.y(), 0);   //  1
           ro[2] = QVector3D(XL.x(), XL.y(), 0);   //2   3
           ro[3] = QVector3D(XR.x(), XR.y(), 0);   //  0
           e1 = ro[3] - ro[2];  //направлениЯ осей координат ГеоСК
           e2 = ro[1] - ro[0];  //во входном снимке
         
           e1 = e1.normalized (); //нормированный базис ГеоСК входного снимка
           e2 = e2.normalized ();
           
           b = ro[0] - ro[2];      //
         
           double e11, e12, e22, e1b, e2b;
           double D, D1, t1;

           // скалaрные произведениа
           e11 = QVector3D::dotProduct(e1,e1);
           e12=QVector3D::dotProduct(e1,e2);
           e22=QVector3D::dotProduct(e2,e2);
           e1b=QVector3D::dotProduct(e1,b);
           e2b=QVector3D::dotProduct(e2,b);
         
           D =-e11*e22+e12*e12;  //преобразование
           D1=-e1b*e22+e2b*e12;
         
           t1=D1/D;
           
           rc=ro[2]+e1*t1; 
           Center=rc.toPointF();

           if(cutting) {
              RR->setCenter (Center);
           }
        }
    }
}

void KrestWindow::createActions( )
{
    connect(action_Quit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionTransform,SIGNAL(triggered()),this,SLOT(run_process()));  //траснсформирование
    connect(action_OpenImage, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(action_OpenImage, SIGNAL(triggered()), this, SLOT(endCut()));
    connect(action_OpenImage, SIGNAL(triggered()), this, SLOT(pickAllKrest()));
    connect(action_OpenImage, SIGNAL(triggered()), this, SLOT(del()));
    action_OpenImage->setShortcut(tr("Ctrl+L"));
    //connect(actionOpenPoints, SIGNAL(triggered()), this, SLOT(openPoints()));
    //actionOpenPoints->setShortcut(tr("Ctrl+P"));
    //connect(action_SavePoints, SIGNAL(triggered()), this, SLOT(savePoints()));
    //action_SavePoints->setShortcut(tr("Ctrl+S"));
    connect(actionSavePointsAs, SIGNAL(triggered()), this, SLOT(savePointsAs()));
    connect(actionCross,    SIGNAL(triggered()), this, SLOT(cursorCross()));
    connect(actionHand,     SIGNAL(triggered()), this, SLOT(cursorHand()));
    connect(actionRubber_2, SIGNAL(triggered()), this, SLOT(cursorRubber()));
    connect(action_RX,      SIGNAL(triggered()), this, SLOT(addKrestRX()));
    connect(action_LX,      SIGNAL(triggered()), this, SLOT(addKrestLX()));
    connect(action_UY,      SIGNAL(triggered()), this, SLOT(addKrestUY()));
    connect(action_DY,      SIGNAL(triggered()), this, SLOT(addKrestDY()));
    connect(actionPickAllKrest, SIGNAL(triggered()), this, SLOT(pickAllKrest()));
    //actionPickAllKrest->setShortcut(tr("Ctrl+A"));
    connect(actionRectangle_2, SIGNAL(triggered()), this, SLOT(cutRect()));
    connect(actionEndCut_2, SIGNAL(triggered()), this, SLOT(endCut()));

    //connect(actionCtrlZ_Cut, SIGNAL(triggered()), this, SLOT(ctrlZ_Cut()));
    //actionCtrlZ_Cut->setShortcut(tr("Ctrl+Z"));
    //connect(actionCtrlZ_AllCut, SIGNAL(triggered()), this, SLOT(ctrlZ_All_Cut()));
    //actionCtrlZ_AllCut->setShortcut(tr("Ctrl+Alt+Z"));

    action_Delete->setShortcut(tr("Del"));
    connect(action_Delete, SIGNAL(triggered()), this, SLOT(del()));
    connect(action_sizePxMm, SIGNAL(triggered()), this, SLOT(SetPixelSize()) );
    connect(action_sizeFrMm, SIGNAL(triggered()), this, SLOT(SetFrameSizeMm()) );
    connect(action_sizeFrPx_2, SIGNAL(triggered()), this, SLOT(SetFrameSizePx()) );
    connect(action_OutImage, SIGNAL(triggered()), this, SLOT(SaveTrAs()) );
    action_OutImage->setShortcut(tr("Ctrl+S"));

    action_SavePoints->setEnabled(false);
    actionRectangle->setEnabled(true);
    actionTransform->setEnabled(false);
    action_OutImage->setEnabled(false);
}
// Задание размера рамки в мм
void KrestWindow::SetFrameSizeMm( ) {
  QIDSFSMm->setDoubleValue(FrameSizeMm);
  QIDSFSMm->show();
   FrameSizePx=(int)ceil(FrameSizeMm/PixelSize);
   if(cutting) {
      RR->SetSize(FrameSizePx);      
   }
   updateActions();  
}


// Задание размера пикселЯ в мм
void KrestWindow::SetPixelSize( ) {
	QInputDialog qid;
	qid.setCancelButtonText(tr("Выход"));
	qid.setOkButtonText(tr("Ввод"));
	qid.getDouble(tr("Введите размер пикселя"), tr("Введите значение, мм:"), PixelSize, 0.001, 2147483647, 3);
	if(PixelSize==0)
	 QMessageBox::information(this, tr("Ошибка"),
	                                 tr("Некорректно введен размер пикселя(размер = 0)"));
}

// Задание размера пикселЯ в пиксеЯх
void KrestWindow::SetFrameSizePx() {
   QIDSFSPx->setIntValue(FrameSizePx);
   QIDSFSPx->show();
   if(cutting) {
      RR->SetSize(FrameSizePx);      
   }
   updateActions(); 
}


void KrestWindow::createMenus( )
{

}


//! Отработка нажатия на кнопку мыши
void KrestWindow::mousePressEvent ( QGraphicsSceneMouseEvent * event) {
   if(event->button()==Qt::LeftButton&&graphicsView->dragMode()==QGraphicsView::RubberBandDrag) {
      startPos=event->pos();
   }
}


//! Отработка отпускания кнопки мыши
void KrestWindow::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event) {
   if(event->button()==Qt::LeftButton&&graphicsView->dragMode()==QGraphicsView::RubberBandDrag) {
      endPos=event->pos();
      QRect RectRBD(startPos.toPoint(),endPos.toPoint());
      graphicsView->fitInView(RectRBD,Qt::KeepAspectRatio);
   }
}


void KrestWindow::ScaleRubberFit(QRect ScaleRect) {
   if(graphicsView->dragMode()==QGraphicsView::RubberBandDrag)
      graphicsView->fitInView(ScaleRect,Qt::KeepAspectRatio) ;
}


//! Создание рамки дл выделени рабочей области
void KrestWindow::cutRect() {
    int thickness;
    cursorCross();
    scene->clearSelection();
    thickness=(int)ceil(QGI_Image->boundingRect().height()/1000);
    if(RR == NULL)
    if(FrameSizePx)
       RR = new RubberRect(QRectF(QPointF(Center.x()-(qreal)FrameSizePx/2,Center.y()-(qreal)FrameSizePx/2),QSizeF((qreal)FrameSizePx,(qreal)FrameSizePx)),QGI_Image->boundingRect(),Center,thickness);
    else
       RR = new RubberRect(QGI_Image->boundingRect(),QGI_Image->boundingRect(),Center,thickness);
    scene->addItem(RR);
    RR->setZValue(1000.0);
    cutting = true;
    iscutting = true;
    RR->cutting = true;
    cut_count = 0;
    actionEndCut_2->setEnabled(true);
    actionRectangle_2->setEnabled(false);
    actionPickAllKrest->setEnabled(false);
}


//! Закончить выделение рабочей области.
void KrestWindow::endCut( ) {
   printf("End cut!\n");
   if ( RR != NULL ) {
      scene -> removeItem ( RR ) ;

      cutting = false;
      RR->cutting = false;
      cursorHand();
      cut_count = 0;
      actionRectangle_2->setEnabled(true);
      actionEndCut_2->setEnabled(false);
      actionPickAllKrest->setEnabled(true);  
   }   
}


// Удаление креста.
void KrestWindow::del()
{
   QList<QGraphicsItem*>items=scene->selectedItems();
      qDeleteAll(items);
}


//! Выбор креста.
Krest *KrestWindow::selectedKrest()
{
    Krest * krest;
    QList<QGraphicsItem *> items = scene->selectedItems( );
    if (items.count() == 1) {
        krest = dynamic_cast<Krest *>(items.first( ));
        return krest;     
    } 
    else {
        return 0;
    }
}


//! Смена курсора.
void KrestWindow::cursorCross() {
    graphicsView->setDragMode(QGraphicsView::NoDrag);
    graphicsView->setCursor(Qt::CrossCursor);    
}

void KrestWindow::cursorHand() {
    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);   
}

void KrestWindow::cursorRubber() {
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);  
}

void KrestWindow::openImage( ) {
   QRegExp tiff_regexp ("(.tiff|.bmp|.jpg|.img|.png|.gif|.tif)");
   
   QString fileName = QFileDialog::getOpenFileName(this,
                           tr("Открыть файл"),"E:/Shtanov/images/");
    if(!fileName.isEmpty()) {
       QString name_buf;       
       nameIn = fileName;
       name_buf=nameIn;       
       nameOut =name_buf.remove(tiff_regexp)+"_tr";
       nameOut =nameOut.split("/").back();
       QGI_Image->set_image( QObject::tr( fileName ) );          
       graphicsView->scaled=1.0;
       graphicsView->scale(graphicsView->scaled, graphicsView->scaled);
       update();
       updateGeometry();
    }
    else
       QMessageBox::information(this, tr("Крест"),
                                     tr("Изображение не было открыто %1.").arg(fileName));
    action_OutImage->setEnabled(true);    
}


void KrestWindow::openPoints( ) {
   const QString fileName = QFileDialog::getOpenFileName(this,
                                 tr("Открыть файл"),"./points");
   QFile file(fileName);
   if(!file.open(QIODevice::ReadOnly))
      QMessageBox::information(this, tr("Крест"),
      tr("Точки не были загружены %1.").arg(fileName));
   else {
      QDataStream in(&file);
      in>>YD>>YU>>XL>>XR;
      addKrestDY(YD);
      addKrestUY(YU);
      addKrestLX(XL);
      addKrestRX(XR);
  }
}
void KrestWindow::savePoints( ) {
    calculCoordKrest();

    QFile file(pathPoint);
    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Крест"),
                                 tr("Точки не были сохранены %1.").arg(pathPoint));
    }
    else{
        QDataStream out(&file);
        out<<YD<<YU<<XL<<XR;
        file.close();
    }
}

void KrestWindow::savePointsAs( ) {
   const QString fileName = QFileDialog::getSaveFileName(this,
                                                         tr("Сохранить точки в файл"), "./points");
   calculCoordKrest();
   pathPoint = fileName;
   QFile file(fileName);
   if(!file.open(QIODevice::WriteOnly)) {
      QMessageBox::information(this, tr("Крест"),
                                     tr("Точки не были сохранены %1.").arg(fileName));
   }
   else {
      QDataStream out(&file);
      out<<YD<<YU<<XL<<XR;
      file.close();
   }
   action_SavePoints->setEnabled(true);
}

void KrestWindow::calculCoordKrest() {
    Krest * krest;
    QList<QGraphicsItem *> items = scene->items( );
    // Вычисление координат крестиков.
    foreach (QGraphicsItem *item, items) {
        if(krest = dynamic_cast<Krest *>(item)) {
            if(krest->text()=="+X") {           // правый крестик "+Х"
                XR = krest->pos();
            }
            if(krest->text()=="-X") {           // левый крестик "-Х"
                XL = krest->pos();
            }
            if(krest->text()=="+Y") {           // верхний крестик "+Y"
                YU = krest->pos();
            }
            if(krest->text()=="-Y") {           // нижний крестик "-Y"
                YD = krest->pos();
            }
        }
    }
}

void KrestWindow::changeCoord(double x, double y) {
   QString str1, str2;

   str1=QObject::tr("%1").arg(int(x));
   str1=QObject::tr("%1").arg(int(y));

   lineEdit_X->setText(str1);
}

void KrestWindow::SaveTrAs( ) {
   QRegExp tiff_regexp ("(.tiff|.bmp|.jpg|.img|.png|.gif|.tif)");
   const QString fileName = QFileDialog::getSaveFileName(this,
                     tr("Сохранить изображение как"),
                     pathOut,tr("Images (*.tif)")            );
   QString fileNameBuf;
   fileNameBuf=fileName;
   fileNameBuf.remove(tiff_regexp);
   nameOut =fileNameBuf.split("/").back();
   pathOut =fileNameBuf.remove(nameOut);
   std::cout<<"pathOut: "<<qPrintable(pathOut)<<std::endl;
   std::cout<<"nameOut: "<<qPrintable(nameOut)<<std::endl;
   actionTransform->setEnabled(true);
}

void KrestWindow::SaveTr( ) {
  
}


void KrestWindow::run_process() {
   progressBar->setValue(0);
   QVector<QPointF> qvqp_x; // {-x, +y, +x, -y}
   calculCoordKrest();
   qvqp_x<<XL<<YU<<XR<<YD;

   QSize out_size(RR->boundingRect().width(),RR->boundingRect().height());
   pThread = new CThread(this) ;
   pThread->progressbar=progressBar ;
   pThread->set_in_name (nameIn) ;
   pThread->set_out_name(pathOut+nameOut) ;
   pThread->set_size_out(out_size);
   pThread->set_point(qvqp_x);
   connect(pThread,SIGNAL(finished()),this,SLOT(StopProcess()));
   pThread->start();
}


void KrestWindow::StopProcess() {
  std::cout<<"StopProcess"<<std::endl;
  if(pThread) pThread->wait();
  std::cout<<" pThread->wait()"<<std::endl;
  if(pThread) delete pThread ;
     pThread = NULL ;
  progressBar->setValue(progressBar->maximum());
  std::cout<<" delete pThread"<<std::endl;
}


/*bool KrestWindow::saveImage(const QString &fileName, const char *fileFormat) {
    return theImage.save(fileName, fileFormat);
} */


/*void KrestWindow::setImage(const QImage &image) {
    QGI_Image= image.convertToFormat(QImage::Format_RGB32);
    update();
    updateGeometry();
}*/


/*
//!отмена действий
void KrestWindow::ctrlZ_Cut() {
   int thickness;
   scene->removeItem(RR);
   delete RR;
   scene->removeItem (QGI_Image);
   if(!cut_count)
      QGI_Image->set_image(nameImgInit);
   else
      QGI_Image->set_image(nameOut+".tif");
   scene->addItem(QGI_Image);
   QGI_Image->setZValue(-1000.0);
   thickness=(int)ceil(QGI_Image->boundingRect().height()/1000);
   RR = new RubberRect(QGI_Image->boundingRect(),Center,thickness);
   scene->addItem(RR);
   RR->setZValue(1000.0);
   --cut_count;
}


void KrestWindow::ctrlZ_All_Cut() {

}*/


/*void KrestWindow::mouseMoveEvent ( QMouseEvent * event )
{
 QString str;
 //c = event->globalX();
// r = event->globalY();
 int c = event->x();
 int r = event->y();
 str=QObject::tr("%1").arg(int(c));

 lineEdit_X->setText(str);

 str=QObject::tr("%1").arg(int(r));
 
 lineEdit_Y->setText(str);

/* Qt::KeyboardModifiers modif;

 modif=event->modifiers();
 
 if (is_move) {
 }
 else {
  double x,y;

  x=event->pos().x();
  y=height()-1-event->pos().y();
  
  if (modif==Qt::ControlModifier) {
   c_R=(x-x0)/scale-dc;
   r_R=(y0-y)/scale-dr;

   emit position_on_shot(c_R,r_R);
//   is_draw_only_curcor=true;
   // перерисовка окна
  
  };
/*  if (modif==Qt::ShiftModifier) {
   c_L=(x-x0)/scale;
   r_L=(y0-y)/scale;

   emit position_on_shot(1, c_L,r_L);
//   is_draw_only_curcor=true;
  
  };  
 //};
} */

/*ock_transform::Block_transform(QPoint YD, QPoint YU, QPoint XL, QPoint XR, int BlockSizeX_OUT, int BlockSizeY_OUT) {
  r0b_OUT = BlockSizeY_OUT/2.0;
  c0b_OUT = BlockSizeX_OUT/2.0;

  ro_block[0]=QVector3D(YD.x(),YD.y(),0);   //!!координаты крестов->QVector3D
  ro_block[1]=QVector3D(YU.x(),YU.y(),0);
  ro_block[2]=QVector3D(XL.x(),XL.y(),0);
  ro_block[3]=QVector3D(XR.x(),XR.y(),0);  

  e1_block=ro_block[3]-ro_block[2];  //направленийа осей координат ГеоСК
  e2_block=ro_block[1]-ro_block[0];  //во входном снимке

  e1_block=e1_block.normalized (); //нормированный базис ГеоСК входного снимка
  e2_block=e2_block.normalized ();

  b_block =ro_block[0]-ro_block[2];  

  e11_block=QVector3D::dotProduct(e1_block,e1_block); //скалaрные произведениа
  e12_block=QVector3D::dotProduct(e1_block,e2_block);
  e22_block=QVector3D::dotProduct(e2_block,e2_block);
  e1b_block=QVector3D::dotProduct(e1_block,b_block);
  e2b_block=QVector3D::dotProduct(e2_block,b_block);

  D_block =-e11_block*e22_block+e12_block*e12_block;  //преобразование
  D1_block=-e1b_block*e22_block+e2b_block*e12_block;

  t1_block=D1_block/D_block;
 
  rc_block=ro_block[2]+e1_block*t1_block; 
}*/


/*void KrestWindow::pressed_button_run() {
   calculCoordKrest();
   //puts("No Error 1");
//---------------ALgorithm---------------------------------
   QString name_in = nameIn;
   QString name_out = nameOut;//QObject::tr("%1_out.tif").arg(name_in);
   QRegExp tiff_regexp ("(.tiff|.bmp|.jpg|.img|.png|.gif|.tif)");
   CGDALRaster rasterIn;
   CGDALRaster rasterOut;
   QPoint RRLU, RRRU, RRRD, RRLD;
   /*RRLU = RR->boundingRect().topLeft().toPoint();
   RRRU = RR->boundingRect().topRight().toPoint();   
   RRRD = RR->boundingRect().bottomRight().toPoint();
   RRLD = RR->boundingRect().bottomLeft().toPoint();  */ 
 //  if(tiff_regexp.search(nameIn)!=-1){
 //     rasterIn.set_path(nameIn) ;
 //     puts("rasterIn.set_path(nameIn) ;");
 //  }
 //  else {
  //    rasterIn.set_path(nameIn+".tiff") ;
 //     puts("rasterIn.set_path(nameIn+.tiff)");      
 //  }

/*  if(!rasterIn.Read(name_in)) {
     rasterIn.Read(name_in+".tif");
      puts("rasterIn.Read(name_in+.tif)");      
  }
  else 
      puts("rasterIn.Read(name_in)");  
  //puts("No Error 2");
  int W_IN = rasterIn.Width() ;
  int H_IN = rasterIn.Height() ;
  int W_OUT = W_IN;
  int H_OUT = H_IN;
 // int W_OUT = 17248;
//  int H_OUT = 18304;  
  double GeoData[6] ;

  QVector3D ro[4],roIN[4];
  QVector3D e1,e2,b,rc;
  QVector3D la1,la2,la;
  double r0,c0;

  ro[0]=QVector3D(YD.x(),YD.y(),0);   //!!координаты крестов->QVector3D
  ro[1]=QVector3D(YU.x(),YU.y(),0);   //  1
  ro[2]=QVector3D(XL.x(),XL.y(),0);   //2   3
  ro[3]=QVector3D(XR.x(),XR.y(),0);   //  0
  e1=ro[3]-ro[2];  //направленийа осей координат ГеоСК
  e2=ro[1]-ro[0];  //во входном снимке

  e1=e1.normalized (); //нормированный базис ГеоСК входного снимка
  e2=e2.normalized ();
  
  b =ro[0]-ro[2];      //

  double e11,e12,e22,e1b,e2b;
  double D,D1,t1;

  e11=QVector3D::dotProduct(e1,e1); //скалaрные произведениа
  e12=QVector3D::dotProduct(e1,e2);
  e22=QVector3D::dotProduct(e2,e2);
  e1b=QVector3D::dotProduct(e1,b);
  e2b=QVector3D::dotProduct(e2,b);

  D =-e11*e22+e12*e12;  //преобразование
  D1=-e1b*e22+e2b*e12;

  t1=D1/D;

  //printf("t1=%lf\n",t1);
  
  rc=ro[2]+e1*t1;   

 /* if(cutting) {
     QVector3D coord_ramk[4],vect_ramk[4];
     QPointF coord_rez[4];
       coord_ramk[0] =QVector3D(RR->coord1.x(),RR->coord1.y(),0);
       coord_ramk[1] =QVector3D(RR->coord2.x(),RR->coord2.y(),0);      
       coord_ramk[2] =QVector3D(RR->coord3.x(),RR->coord3.y(),0);        
       coord_ramk[3] =QVector3D(RR->coord4.x(),RR->coord4.y(),0); 
          for(int l=0;l<4;++l) {
            vect_ramk[l]=rc-coord_ramk[l];
            coord_rez[l].setX(fabs(QVector3D::dotProduct(vect_ramk[l],e1)));
            coord_rez[l].setY(fabs(QVector3D::dotProduct(vect_ramk[l],e2)));            
          }
     double maxX,maxY;
      maxX=coord_rez[0].x( );
      maxY=coord_rez[0].y( );    
      for(int k=0;k<4;++k) {
        if(coord_rez[k].x( ) > maxX) {
             maxX = coord_rez[k].x( );
        }
        if(coord_rez[k].y( ) > maxY) {
             maxY = coord_rez[k].y( );
        }        
     }
     H_OUT =(int) ceil(2*maxY);
     W_OUT =(int) ceil(2*maxX);     
  } 
*/

  // H_OUT = H_IN; 
  //  W_OUT = W_IN;
  //if(!rasterOut.Read(name_out)) {
/*  if(!rasterOut.CreateGeoTIFF( name_out, W_OUT, H_OUT,
                                GeoData, rasterIn.GetProjectionData() )) {
      puts("rasterOut.CreateGeoTIFF( name_out, truble");
      return;
  }
  else
      puts("rasterOut.CreateGeoTIFF( name_out,");  
  //puts("No Error 3");
    //размеры блока по умолчанию
  int BlockSizeX_OUT =  MAX_SIZE_BLOCK; //l
  int BlockSizeY_OUT =  MAX_SIZE_BLOCK;
  //  int W,H;
    // кол-во блоков
  int NumBlocksX_OUT = (int) ceil( double(W_OUT) / double(BlockSizeX_OUT) ) ;
  int NumBlocksY_OUT = (int) ceil( double(H_OUT) / double(BlockSizeY_OUT) ) ;
  //рассчет размеров блока (N_block*h_block = H_raster) OUT
  if ( BlockSizeX_OUT >= W_OUT)
    BlockSizeX_OUT= W_OUT;
  else
    BlockSizeX_OUT= (int)((double)W_OUT /(double) NumBlocksX_OUT);
  if ( BlockSizeY_OUT >= H_OUT)
    BlockSizeY_OUT= H_OUT;
  else
    BlockSizeY_OUT= (int)((double)H_OUT /(double) NumBlocksY_OUT);

  //printf("\nHIN %d WIN %d HOUT %d WOUT %d BXOUT %d BYOUT %d",H_IN,W_IN,H_OUT,W_OUT,BlockSizeX_OUT,BlockSizeY_OUT);
  //printf("\nNumBlocksX_OUT %d NumBlocksY_OUT %d",NumBlocksX_OUT,NumBlocksY_OUT);
  r0=H_OUT/2.0;  //координаты центра выходного снимка
  c0=W_OUT/2.0;  

  double cp,rp;
  double dc,dr;  
  int icp,irp;
  QVector3D point;  
  //QImage ijBlock_OUT(BlockSizeX_OUT,BlockSizeY_OUT,QImage::Format_Indexed8);
  QImage ijBlock_OUT(BlockSizeX_OUT,BlockSizeY_OUT,QImage::Format_RGB32);
        //ijBlock_OUT.setNumColors(256) ;

        /*for( int i = 0 ; i < 256 ; i ++ ) {
            ijBlock_OUT.setColor( i, QColor(i,i,i).rgb( ) ) ;     
        } ;*/
/*  puts("No Error 4");
  QPoint ijPosB ;
//цикл по всем блокам длЯ поворота растра
  for( int iBlock = 0 ; iBlock < NumBlocksY_OUT; iBlock ++ ) {
    for( int jBlock = 0 ; jBlock < NumBlocksX_OUT; jBlock ++ ) {
      progressBar->setValue((int)((double)(iBlock*NumBlocksX_OUT+jBlock)/(double)NumBlocksX_OUT*NumBlocksY_OUT));
//очередные координаты блока
   QPoint ijPos(jBlock*BlockSizeX_OUT, iBlock*BlockSizeY_OUT) ; 
//читаем блок результирующей карты
   //ijBlock_OUT= rasterOut.GetQImage(ijPos, BlockSizeX_OUT, BlockSizeY_OUT) ;
      
   QPoint pb[4];
   pb[0] = ijPos;
   pb[1] = ijPos+QPoint(BlockSizeX_OUT,0);               //12
   pb[2] = ijPos+QPoint(BlockSizeX_OUT,BlockSizeY_OUT);  //43 
   pb[3] = ijPos+QPoint(0,BlockSizeY_OUT); //координаты углов блока в выходном растре

  // rc = QVector3D(W_IN/2,H_IN/2,0);   
   
   QPointF pbIN[4];
   double dcIN[4],drIN[4];
     for( int k = 0; k < 4 ; ++k ) {
            dcIN[k]=pb[k].x()-c0;
            drIN[k]=r0-pb[k].y();
            point=rc+e1*dcIN[k]+e2*drIN[k];
            pbIN[k] = point.toPoint();     //координаты углов блока во входном растре
      }
     for( int k = 0; k < 4 ; ++k ) {
        if(pbIN[k].x()<0)pbIN[k].setX(0);
        if(pbIN[k].y()<0)pbIN[k].setY(0);  
        if(pbIN[k].x()>W_IN)pbIN[k].setX(W_IN);   
        if(pbIN[k].y()>H_IN)pbIN[k].setY(H_IN);        
      }
    double maxX,maxY,minX,minY;
    maxX=pbIN[0].x();
    maxY=pbIN[0].y();    
    minX=pbIN[0].x();    
    minY=pbIN[0].y();    
      for(int k=0;k<4;++k) {
        if(pbIN[k].x() > maxX) {
          if(pbIN[k].x() < W_IN)
             maxX = pbIN[k].x();
          else
             maxX = W_IN;
        }
        if(pbIN[k].y() > maxY) {
          if(pbIN[k].y()<H_IN)
             maxY = pbIN[k].y();   
          else
             maxY = H_IN;               
        }
        if(pbIN[k].x() < minX) {
          if(pbIN[k].x()>0)
             minX = pbIN[k].x();
          else
             minX = 0;
        }
        if(pbIN[k].y() < minY) {
          if(pbIN[k].y()>0)
             minY = pbIN[k].y();
          else
             minY = 0;
        }
      }
  int BlockSizeX_IN = ceil(maxX) - floor(minX); //l
  int BlockSizeY_IN = ceil(maxY) - floor(minY);
  //QImage ijBlock_IN(BlockSizeX_IN,BlockSizeY_IN,QImage::Format_Indexed8);
  QImage ijBlock_IN(BlockSizeX_IN,BlockSizeY_IN,QImage::Format_RGB32);  
  QPoint ijPosIN(floor(minX),floor(minY));
 // printf("\nijPosXIN %d ijPosYIN %d",ijPosIN.x(),ijPosIN.y());
 // printf("\nmaxX %d minX %d maxY %d minY %d",maxX,minX,maxY,minY);
 // getchar();  
      fprintf(stdout,"block: iBlock = %d : jBlock = %d\n", iBlock, jBlock);
      // читаем блок исходного растра
        //puts("No Error 5");
        ijBlock_IN= rasterIn.GetQImageRGB32(ijPosIN, BlockSizeX_IN, BlockSizeY_IN);
        //puts("No Error 6");
        //ijBlock_IN.save("E:\\Shtanov\\images\\test\\ijBlock_IN.bmp");
       //QString name_test = QObject::tr("E:/Shtanov/images/test/img_%1_%2.bmp").arg(iBlock).arg(jBlock);
      //ijBlock_IN.save(name_test);
  //  QString name_testOUT = QObject::tr("E:/Shtanov/images/test/img_%1_%2.bmp").arg(iBlock).arg(jBlock);      
   //   ijBlock_IN.save(name_test); 

       
    for(int r=0;r<BlockSizeY_OUT;r++) {
       for(int c=0;c<BlockSizeX_OUT;c++) {         
//координаты точек блока внутри растра = координаты блока + координаты внутри блока
            ijPosB = ijPos + QPoint(c,r);
          //  printf("ijPosX %d ijPosY %d ijPosBX %d ijPosBY %d",ijPos.x(),ijPos.y(),ijPosB.x(),ijPosB.y());
  //        dc=c-c0;
   //         dr=r0-r;

            dc=ijPosB.x()-c0;
            dr=r0-ijPosB.y();


             
            point=rc+e1*dc+e2*dr; //координаты во входном снимке
            cp=point.x();
            rp=point.y();
            icp = (int)roundD(cp);
            irp = (int)roundD(rp);
          //  printf("\nCP %d RP %d ij_IN_X %d ij_IN_Y %d",icp,irp,ijPosIN.x(),ijPosIN.y());
           // getchar();
           // ijPos_IN = point.toPoint();      
           
            int color;
           // QRgb colRgb;
       //      if( cp < BlockSizeX_IN && cp >= 0 ) {
         //       if( rp < BlockSizeY_IN && rp >= 0 )
         
             if( icp < W_IN && icp >= 0 ) {
                if( irp < H_IN && irp >= 0 ) { 
                  if(RR->boundingRect().contains(icp,irp) ) {
                     if(icp-ijPosIN.x()<BlockSizeX_IN&&irp-ijPosIN.y()<BlockSizeY_IN)
                       color = qGray(ijBlock_IN.pixel(icp-ijPosIN.x(),irp-ijPosIN.y()));
                     //  color = ijBlock_IN.pixelIndex(icp-ijPosIN.x(),irp-ijPosIN.y());
                     else
                       color = qGray(ijBlock_IN.pixel(icp-ijPosIN.x()-1,irp-ijPosIN.y()-1));
                       //color = ijBlock_IN.pixelIndex(icp-ijPosIN.x()-1,irp-ijPosIN.y()-1);
                  }
                  else
                    color = qGray(0,0,0);
                }
                else
                  color = qGray(0,0,0);
             }
             else
                color = qGray(0,0,0);

             ijBlock_OUT.setPixel(c,r,qRgb(color,color,color));
            
        }
      }
       //ijBlock_OUT.save("E:\\Shtanov\\images\\test\\ijBlock_OUT.bmp");
      
       rasterOut.WriteRGB32( ijPos, ijBlock_OUT );
       //puts("No Error 7");
       //rasterOut.create_overviews(name_out);
        
      }
    }
  
}
*/
/*void KrestWindow::pressed_button_run2() {
   calculCoordKrest();
   //puts("No Error 1");
//---------------ALhorithm---------------------------------
   QString name_in = nameIn;
   QString name_out = pathOut+nameOut;

   //std::cout<<qPrintable(name_in)<<std::endl;
   CGDALRaster rasterIn(name_in);
   CGDALRaster rasterOut;

  /*if(!rasterIn.Read(name_in)) {
     rasterIn.Read(name_in+".tif");
     puts("rasterIn.Read(name_in+.tif)");      
  }
  else 
      puts("rasterIn.Read(name_in)");  
  //puts("No Error 2");*/
  
/*  int W_IN = rasterIn.Width() ;
  int H_IN = rasterIn.Height() ;
  int W_OUT = qMax(W_IN,H_IN);
  int H_OUT = qMax(W_IN,H_IN);

   if(iscutting) {
      QPoint RRLU, RRRU, RRRD, RRLD;
      RRLU = RR->boundingRect().topLeft().toPoint();
      RRRU = RR->boundingRect().topRight().toPoint();   
      RRRD = RR->boundingRect().bottomRight().toPoint();
      RRLD = RR->boundingRect().bottomLeft().toPoint();
      W_OUT= RR->boundingRect().width();
      H_OUT= RR->boundingRect().height();      
   }
  
  double GeoData[6] ;

  QVector3D ro[4],roIN[4];
  QVector3D e1,e2,b,rc;
  QVector3D la1,la2,la;
  double r0,c0;

  ro[0]=QVector3D(YD.x(),YD.y(),0);   //!!координаты крестов->QVector3D
  ro[1]=QVector3D(YU.x(),YU.y(),0);   //  1
  ro[2]=QVector3D(XL.x(),XL.y(),0);   //2   3
  ro[3]=QVector3D(XR.x(),XR.y(),0);   //  0
  e1=ro[3]-ro[2];  //направленийа осей координат ГеоСК
  e2=ro[1]-ro[0];  //во входном снимке

  e1=e1.normalized (); //нормированный базис ГеоСК входного снимка
  e2=e2.normalized ();
  
  b =ro[0]-ro[2];      //

  double e11,e12,e22,e1b,e2b;
  double D,D1,t1;

  e11=QVector3D::dotProduct(e1,e1); //скалaрные произведениа
  e12=QVector3D::dotProduct(e1,e2);
  e22=QVector3D::dotProduct(e2,e2);
  e1b=QVector3D::dotProduct(e1,b);
  e2b=QVector3D::dotProduct(e2,b);

  D =-e11*e22+e12*e12;  //преобразование
  D1=-e1b*e22+e2b*e12;

  t1=D1/D;

  //printf("t1=%lf\n",t1);
  
  rc=ro[2]+e1*t1;   
  //создание выходного растра в формате .TIF
  if(!rasterOut.CreateGeoTIFF( name_out, W_OUT, H_OUT,
                                GeoData, rasterIn.GetProjectionData() )) {
      puts("rasterOut.CreateGeoTIFF( name_out, truble");
      return;
  }
  else
      puts("rasterOut.CreateGeoTIFF( name_out,");  
  //puts("No Error 3");
  
    //размеры блока по умолчанию
  int BlockSizeX_OUT =  MAX_SIZE_BLOCK; //l
  int BlockSizeY_OUT =  MAX_SIZE_BLOCK;
    // кол-во блоков
  int NumBlocksX_OUT = (int) ceil( double(W_OUT) / double(BlockSizeX_OUT) ) ;
  int NumBlocksY_OUT = (int) ceil( double(H_OUT) / double(BlockSizeY_OUT) ) ;
  //рассчет размеров блока (N_block*h_block = H_raster) OUT
  if ( BlockSizeX_OUT >= W_OUT)
    BlockSizeX_OUT= W_OUT;
  else
    BlockSizeX_OUT= (int)((double)W_OUT /(double) NumBlocksX_OUT);
  if ( BlockSizeY_OUT >= H_OUT)
    BlockSizeY_OUT= H_OUT;
  else
    BlockSizeY_OUT= (int)((double)H_OUT /(double) NumBlocksY_OUT);

  //printf("\nHIN %d WIN %d HOUT %d WOUT %d BXOUT %d BYOUT %d",H_IN,W_IN,H_OUT,W_OUT,BlockSizeX_OUT,BlockSizeY_OUT);
  //printf("\nNumBlocksX_OUT %d NumBlocksY_OUT %d",NumBlocksX_OUT,NumBlocksY_OUT);
  r0=H_OUT/2.0;  //координаты центра выходного снимка
  c0=W_OUT/2.0;  

  double cp,rp;
  double dc,dr;  
  int icp,irp;
  QVector3D point;  

  QImage ijBlock_OUT(BlockSizeX_OUT,BlockSizeY_OUT,QImage::Format_RGB32);

  //puts("No Error 4");
  QPoint ijPosB ;
//цикл по всем блокам длЯ поворота растра
  for( int iBlock = 0 ; iBlock < NumBlocksY_OUT; iBlock ++ ) {
    for( int jBlock = 0 ; jBlock < NumBlocksX_OUT; jBlock ++ ) {
       progressBar->setValue((int)((double)(iBlock*NumBlocksX_OUT+jBlock)/(double)NumBlocksX_OUT*NumBlocksY_OUT)*100);
       //очередные координаты блока в выходном растре
       QPoint ijPos(jBlock*BlockSizeX_OUT, iBlock*BlockSizeY_OUT) ; 
       //читаем блок результирующей карты
       QPoint pb[4];
       pb[0] = ijPos;
       pb[1] = ijPos+QPoint(BlockSizeX_OUT,0);               //12
       pb[2] = ijPos+QPoint(BlockSizeX_OUT,BlockSizeY_OUT);  //43 
       pb[3] = ijPos+QPoint(0,BlockSizeY_OUT); //координаты углов блока в выходном растре
    
       QPointF pbIN[4];
       double dcIN[4],drIN[4];
       for( int k = 0; k < 4 ; ++k ) {
           dcIN[k]=pb[k].x()-c0;
           drIN[k]=r0-pb[k].y();
           point=rc+e1*dcIN[k]+e2*drIN[k];
           pbIN[k] = point.toPoint();     //координаты углов блока во входном растре
       }
       for( int k = 0; k < 4 ; ++k ) {
          if(pbIN[k].x()<0)pbIN[k].setX(0);
          if(pbIN[k].y()<0)pbIN[k].setY(0);  
          if(pbIN[k].x()>W_IN)pbIN[k].setX(W_IN);   
          if(pbIN[k].y()>H_IN)pbIN[k].setY(H_IN);        
       }
       double maxX,maxY,minX,minY;
       maxX=pbIN[0].x();
       maxY=pbIN[0].y();    
       minX=pbIN[0].x();    
       minY=pbIN[0].y();    
       for(int k=0;k<4;++k) {
          if(pbIN[k].x() > maxX) {
             if(pbIN[k].x() < W_IN)
                maxX = pbIN[k].x();
             else
                maxX = W_IN;
          }
          if(pbIN[k].y() > maxY) {
             if(pbIN[k].y()<H_IN)
                maxY = pbIN[k].y();   
             else
                maxY = H_IN;               
          }
          if(pbIN[k].x() < minX) {
             if(pbIN[k].x()>0)
                minX = pbIN[k].x();
             else
                minX = 0;
          }
          if(pbIN[k].y() < minY) {
             if(pbIN[k].y()>0)
                minY = pbIN[k].y();
             else
                minY = 0;
          }
       }
       //размеры блока во входном растре
       int BlockSizeX_IN = ceil(maxX) - floor(minX); 
       int BlockSizeY_IN = ceil(maxY) - floor(minY);
       //QImage ijBlock_IN(BlockSizeX_IN,BlockSizeY_IN,QImage::Format_RGB32);  
       QImage ijBlock_IN(BlockSizeX_IN,BlockSizeY_IN, QImage::Format_Indexed8);//блок входного растра, чб, тк растр чб
       QPoint ijPosIN(floor(minX),floor(minY)); 
       fprintf(stdout,"block: iBlock = %d : jBlock = %d\n", iBlock, jBlock);
       // читаем блок исходного растра
       //puts("No Error 5");
       //ijBlock_IN= rasterIn.GetQImageRGB32(ijPosIN, BlockSizeX_IN, BlockSizeY_IN);
       ijBlock_IN= rasterIn.GetQImageGray(ijPosIN, BlockSizeX_IN, BlockSizeY_IN);        
       //puts("No Error 6");
       
       for(int r=0;r<BlockSizeY_OUT;r++) {
          for(int c=0;c<BlockSizeX_OUT;c++) {         
             //координаты точек блока внутри растра = координаты блока + координаты внутри блока
             ijPosB = ijPos + QPoint(c,r); //в выходном растре

             dc=ijPosB.x()-c0;   //относительно центра
             dr=r0-ijPosB.y();

             point=rc+e1*dc+e2*dr; //координаты во входном снимке
             cp=point.x();  //координаты во входном снимке
             rp=point.y();
             icp = (int)roundD(cp); //координаты во входном снимке в типе инт
             irp = (int)roundD(rp);

             int color;
             QRgb qrgb_color;

             if( icp < W_IN && icp >= 0 ) {
                if( irp < H_IN && irp >= 0 ) { 
                   if(iscutting){
                      if(RR->boundingRect().contains(icp,irp))                       
                         if(icp-ijPosIN.x()<BlockSizeX_IN&&irp-ijPosIN.y()<BlockSizeY_IN)
                            color = qGray(ijBlock_IN.pixel(icp-ijPosIN.x(),irp-ijPosIN.y()));
                            //qrgb_color=ijBlock_IN.pixel(icp-ijPosIN.x(),irp-ijPosIN.y());
                         else
                            //color = qGray(ijBlock_IN.pixel(icp-ijPosIN.x()-1,irp-ijPosIN.y()-1));
                            //qrgb_color=ijBlock_IN.pixel(icp-ijPosIN.x()-1,irp-ijPosIN.y()-1);
                            color = qGray(0,0,0);
                       else
                            color = qGray(0,0,0);                        
                   }
                   else if(!iscutting) {
                      if(icp-ijPosIN.x( )<BlockSizeX_IN&&irp-ijPosIN.y()<BlockSizeY_IN)
                         color = qGray(ijBlock_IN.pixel(icp-ijPosIN.x(),irp-ijPosIN.y()));
                         //qrgb_color=ijBlock_IN.pixel(icp-ijPosIN.x(),irp-ijPosIN.y());
                      else
                         //color = qGray(ijBlock_IN.pixel(icp-ijPosIN.x()-1,irp-ijPosIN.y()-1));
                         //qrgb_color=ijBlock_IN.pixel(icp-ijPosIN.x()-1,irp-ijPosIN.y()-1);
                         color = qGray(0,0,0);
                   }
                   else
                     color = qGray(0,0,0);
                     //qrgb_color=qRgb(0,0,0);
                }
                else
                  color = qGray(0,0,0);
                  //qrgb_color=qRgb(0,0,0);                
             }
             else
                color = qGray(0,0,0);
                //qrgb_color=qRgb(0,0,0);             

             ijBlock_OUT.setPixel(c,r,qRgb(color,color,color));
             //ijBlock_OUT.setPixel(c,r,color);                
             //ijBlock_OUT.setPixel(c,r,qrgb_color);

         }
       }
       rasterOut.WriteRGB32( ijPos, ijBlock_OUT );
       //puts("No Error 7");
    }
  }
  rasterOut.create_overviews( );
}
*/


/*void KrestWindow::cutImage( ) {
  
 QString name_buf;

 CGDALRaster  rasterOut;
 CGDALRaster  rasterIn;

  if(cut_count==0)
    rasterIn.set_path(nameIn);
  else
    rasterIn.set_path(nameIn+".tif");
  int W_IN = rasterIn.Width() ;
  int H_IN = rasterIn.Height() ;
  int W_OUT =(int) RR->sizeF.width();
  int H_OUT =(int) RR->sizeF.height();
  int _NumBands = 1;
  int _nPixelSpace = 0 ;
  double GeoData[6] ;
  for( int i = 0 ; i < 6 ; i ++ ) GeoData[i] = 0 ;
  puts("No Error 1");
  
     if(!rasterOut.CreateGeoTIFF( nameOut, W_OUT, H_OUT, 
                                  GeoData, rasterIn.GetProjectionData() ))
              puts("Dont open image");                          

  puts("No Error 2");
  int BlockSizeX = MAX_SIZE_BLOCK; //l
  int BlockSizeY = MAX_SIZE_BLOCK;
  int NumBlocksX=1, NumBlocksY=1;
    // кол-во блоков
  if ( BlockSizeX >= W_OUT) {
    BlockSizeX=W_OUT;
    puts("\nBlockSizeX=W_OUT;");
   }
  else {
    NumBlocksX=(int)floor( double(W_OUT) / double(BlockSizeX) ) +1;
    BlockSizeX=(int)(double)W_OUT /(double) NumBlocksX;
  }
  if ( BlockSizeY >= H_OUT) {
    BlockSizeY=H_OUT;
    puts("\nBlockSizeY=W_OUT;");    
  }
  else {
    NumBlocksY =  (int)floor( double(H_OUT) / double(BlockSizeY) ) +1;
    BlockSizeY=(int)(double)H_OUT  /(double) NumBlocksY;
  }
  printf("HIN %d WIN %d HOUT %d WOUT %d BXOUT %d BYOUT %d\n",H_IN,W_IN,H_OUT,W_OUT,BlockSizeX,BlockSizeY);
  printf("\nNumBlocksX %d NumBlocksY %d",NumBlocksX,NumBlocksY); 

  QImage ijBlock;
  
  for( int iBlock = 0 ; iBlock < NumBlocksY; iBlock ++ ) {
    for( int jBlock = 0 ; jBlock < NumBlocksX; jBlock ++ ) {
       //очередные координаты блока
       QPoint ijPos(jBlock*BlockSizeX, iBlock*BlockSizeY) ; 
       //читаем блок результирующей карты
       ijBlock= rasterIn.GetQImage(ijPos + RR->coord1.toPoint(), BlockSizeX, BlockSizeY) ;
       puts("NO ERROR r");
       rasterOut.Write( ijPos, ijBlock );
       puts("NO ERROR w");
    }
  }

  scene->removeItem (QGI_Image);

  puts("YA tut");
  QGI_Image->set_image(nameOut+".tif");
  QGI_Image->setPos(RR->coord1);
  printf("\n WQGI %f HQGI %f",QGI_Image->boundingRect().width(),QGI_Image->boundingRect().height());
  puts("YA tut");
  scene->addItem    (QGI_Image);
  QGI_Image->setZValue(-1000.0);
  printf("\nCUT_COUNT %d",cut_count);
  if(!cut_count) {
     nameIn = nameOut ;
     nameOut= nameOut2;
  }
  else {
     name_buf = nameIn;
     nameIn = nameOut;
     nameOut = name_buf;
 }
  
  cut_count++;   
}


void KrestWindow::cutImage2( ) {
 QString name_buf;

 CGDALRaster  rasterOut;
 CGDALRaster  rasterIn;

    rasterIn.set_path(nameIn);

  int W_IN = rasterIn.Width() ;
  int H_IN = rasterIn.Height() ;
  int W_OUT = W_IN;
  int H_OUT = H_IN;
  int _NumBands = 1;
  int _nPixelSpace = 0;
  double GeoData[6] ;
  for( int i = 0 ; i < 6 ; i ++ ) GeoData[i] = 0 ;
  puts("No Error 1");
  
     if(!rasterOut.CreateGeoTIFF( nameOut, W_OUT, H_OUT, 
                                  GeoData, rasterIn.GetProjectionData() ))
              puts("Dont open image");                          
     
  W_OUT =(int) RR->sizeF.width();
  H_OUT =(int) RR->sizeF.height();
  int BlockSizeX = MAX_SIZE_BLOCK; //l
  int BlockSizeY = MAX_SIZE_BLOCK;
  int NumBlocksX=1, NumBlocksY=1;
    // кол-во блоков
  if ( BlockSizeX >= W_OUT) {
    BlockSizeX=W_OUT;
    puts("\nBlockSizeX=W_OUT;");
   }
  else {
    NumBlocksX=(int)ceil( double(W_OUT) / double(BlockSizeX) );
    BlockSizeX=(int)(double)W_OUT /(double) NumBlocksX;
  }
  if ( BlockSizeY >= H_OUT) {
    BlockSizeY=H_OUT;
    puts("\nBlockSizeY=W_OUT;");    
  }
  else {
    NumBlocksY =  (int)ceil( double(H_OUT) / double(BlockSizeY) );
    BlockSizeY=(int)(double)H_OUT  /(double) NumBlocksY;
  }
  printf("HIN %d WIN %d HOUT %d WOUT %d BXOUT %d BYOUT %d\n",H_IN,W_IN,H_OUT,W_OUT,BlockSizeX,BlockSizeY);
  printf("\nNumBlocksX %d NumBlocksY %d",NumBlocksX,NumBlocksY); 

  QImage ijBlock(BlockSizeX,BlockSizeY,QImage::Format_Indexed8);
 
  for( int iBlock = 0 ; iBlock < NumBlocksY; iBlock ++ ) {
    for( int jBlock = 0 ; jBlock < NumBlocksX; jBlock ++ ) {
       //очередные координаты блока
       QPoint ijPos(jBlock*BlockSizeX, iBlock*BlockSizeY) ; 
       //читаем блок результирующей карты
       ijBlock= rasterIn.GetQImage(ijPos + RR->coord1.toPoint( ), BlockSizeX, BlockSizeY) ;
       puts("NO ERROR r");
       ijBlock.save("E:\\Shtanov\\images\\test\\ijBlock_CUT.bmp");
       rasterOut.Write( ijPos+ RR->coord1.toPoint( ), ijBlock );
       puts("NO ERROR w");
    }
  }

  scene->removeItem (QGI_Image);

  QGI_Image->set_image(nameOut+".tif");
  printf("\n%s",nameOut);

  scene->addItem    (QGI_Image);
  QGI_Image->setZValue(-1000.0);
}
//!отображение рабочей области
//точнее попытка создать черную область вокруг рамки
void KrestWindow::cutImage3() {

 QPixmap pix_scene(RR->boundingRect().toRect().size());

 QPainter scene_painter(&pix_scene);

 QGraphicsPixmapItem *pix_item;
 pix_item = scene->addPixmap(pix_scene);
 pix_item->setZValue(-999.0); 
}
*/


/*void KrestWindow::changeCoord() {
   QString str1,str2;

   str1=QObject::tr("%1").arg(int(scene->cursor_scene_pos().x()));
   str1=QObject::tr("%1").arg(int(scene->cursor_scene_pos().y()));

   lineEdit_X->setText(str1);

   lineEdit_Y->setText(str2);
}
/*void KrestWindow::pressed_button_run() {
   calculCoordKrest();
//---------------ALhorithm---------------------------------
 /*QString name_in = nameIn;
 QString name_out = nameIn+"out.img";

 CGDALRaster rasterIn(nameIn) ;
 CGDALRaster rasterOut(name_out) ;

  int W_IN = rasterIn.Width() ;
  int H_IN = rasterIn.Height() ;
  int W_OUT = ( W_IN + H_IN );
  int H_OUT = ( W_IN + H_IN );
  bool isNew = false ;
  if(rasterOut.isNull()) {
    isNew = true ;
    double GeoData[6] ;
    rasterIn.GetGeoTransform(GeoData) ;

    if( !rasterOut.CreateGeoTIFF( name_out, W_OUT, H_OUT, 
                                GeoData, rasterIn.GetProjectionData() ) ) printf("Gdal fail") ;
  } ;

  QMatrix M_IN = rasterIn.GetTransformMatrix() ;
//размеры блока по умолчанию
  int BlockSizeX = 1024 ;
  int BlockSizeY = 1024 ;
    // кол-во блоков
  int NumBlocksX = (int) ceil( double(W_IN) / double(BlockSizeX) ) ;
  int NumBlocksY = (int) ceil( double(H_IN) / double(BlockSizeY) ) ;
  //рассчет размеров блока (N_block*h_block = H_raster)
  if ( BlockSizeX > W_IN )
    BlockSizeX = W_IN;
  else
    BlockSizeX = W_IN / NumBlocksX;
  if ( BlockSizeY > H_IN )
    BlockSizeY = H_IN;
  else
    BlockSizeY = H_IN / NumBlocksY;
  */
 
// QString name_point;
/* int w_in,h_in,w_out,h_out;
// int a;
 int d;

// FILE *f;

// int k;

// double s;

// s=0.02795;

 //a=3578;
// w=h=(2*a+1);
 

// for(k=11;k<=15;k++) {
  QVector3D ro[4];
  QVector3D e1,e2,b,rc;
                                       //img_doc_1
QString  name_in= QObject::tr("E:/Shtanov/images/img_doc_1.bmp");//.arg(k);
QString  name_out=QObject::tr("E:/Shtanov/images/img_doc_1_out.bmp");//.arg(k);
//  name_in= QObject::tr("E:/Shtanov/images/1.bmp");//.arg(k);
  //name_out=QObject::tr("E:/Shtanov/images/1_out.bmp");//.arg(k);  
//  name_in= QObject::tr("E:/Shtanov/images/image.bmp");//.arg(k);
//  name_out=QObject::tr("E:/Shtanov/images/image_out.bmp");//.arg(k);  
//  name_in=QObject::tr("E:/Shtanov/images/air_rot.bmp");//.arg(k);
//  name_out=QObject::tr("E:/Shtanov/images/air_rot_out.bmp");//.arg(k);
  
//  name_point=QObject::tr("Y:/test_afa2/raster_i/s_%1.txt").arg(k);

  

  QImage im;
  QImage im_out;

  im.load(name_in);
  //im_out=im;

  w_in=im.width();
  h_in=im.height();
  printf("w=%d h=%d\n",w_in,h_in);
   //w_out=h_out=(w_in+h_in+1);
  w_out=h_out=(((w_in+h_in) / 0.9) + 1);

  printf("w=%d h=%d\n",w_out,h_out);
  getchar();

  im_out=QImage(w_out,h_out,im.format());

  im_out.fill(0);

  int r,c;
  double r0,c0;

  

  r0=im_out.height()/2.0;
  c0=im_out.width()/2.0;

  /*
  c0p=1152.709433;
  r0p=1155.999349;
  m11=0.999939;
  m12=0.011016;
  m21=-0.011122;
  m22=1.000121;
  */
/*  f=fopen(name_point.toLocal8Bit().data(),"r");
  {
   double c,r;                       //!! загрузка точек из файла,
   int k;                            //!! мне не надо ибо все уже есть
   for(k=0;k<4;k++) {
    fscanf(f,"%lf %lf",&c,&r);

    ro[k]=QVector3D(c,r,0);
   };
  };
  fclose(f);  */

  //printf("k=%d c0=%lf r0=%lf\n",k,c0,r0);

 /*     ro[0]=QVector3D(YD.x(),YD.y(),0);   //!!координаты точек->QVector3D
      ro[1]=QVector3D(YU.x(),YU.y(),0);
      ro[2]=QVector3D(XL.x(),XL.y(),0);
      ro[3]=QVector3D(XR.x(),XR.y(),0);      
  
  e1=ro[3]-ro[2];
  e2=ro[1]-ro[0];

  e1=e1.normalized ();
  e2=e2.normalized ();
  b=ro[0]-ro[2];

  double e11,e12,e22,e1b,e2b;
  double D,D1,t1;

  e11=QVector3D::dotProduct(e1,e1);
  e12=QVector3D::dotProduct(e1,e2);
  e22=QVector3D::dotProduct(e2,e2);
  e1b=QVector3D::dotProduct(e1,b);
  e2b=QVector3D::dotProduct(e2,b);

  D =-e11*e22+e12*e12;  //преобразование
  D1=-e1b*e22+e2b*e12;

  t1=D1/D;

//  printf("t1=%lf\n",t1);
  
  rc=ro[2]+e1*t1;

  printf("cc=%lf rc=%lf\n",rc.x(),rc.y());
  getchar();       


  for(r=1;r<h_out;r++) {
   for(c=1;c<w_out;c++) {
  //    printf("w=%d h=%d\n",w_out,h_out);
//        getchar();  
    double cp,rp;
    double dc,dr;
    QVector3D point;

    dc=c-c0;
    dr=r0-r;

    point=rc+e1*dc+e2*dr;
    cp=point.x();
    rp=point.y();

    //if ((cp>=d)&&(rp>=d)&&(cp<im.width()-d)&&(rp<im.height()-d)) {
     uint color;
    QRgb colRgb;
     if( cp < w_in && cp >= 0 ) {
        if( rp < h_in && rp >= 0 ) 
          colRgb=im.pixel(cp,rp);
        //  colRgb=qRgb(255,255,255);
        else
          colRgb=qRgb(0,0,0);
        }
     else
        colRgb=qRgb(0,0,0);
     
     im_out.setPixel(c,r,colRgb);
//     puts("Error!!!!!");
    //};
   };
  };

  printf("End process.Saved...");
  im_out.save(name_out);
  printf("End Saved");
//  Ctiff_util::convert_QImage_to_TILE_JPEQ_TIFF(name_out,im_out,100,128); 
 //};
//---------------End ALhorithm---------------------------------
}

/*
void KrestWindow::pressed_button_run() {
   calculCoordKrest();
//---------------ALhorithm---------------------------------
 //QString name_in = nameIn;
// QString name_out = nameIn+"out.bmp";
 QString name_in = "E:/Shtanov/images/img_doc_1.bmp";
 QString name_out = "E:/Shtanov/images/img_doc_1_out_1.bmp";
         puts("ya tuta");
 nameIn = name_in;

 CGDALRaster rasterIn(nameIn) ;
 puts("ya tuta CGDALRaster rasterIn(nameIn)");

 //CGDALRaster rasterOut(name_out) ;
 puts("ya tuta CGDALRaster rasterOut(name_out)");
  int W_IN = rasterIn.Width() ;
  int H_IN = rasterIn.Height() ;
  int W_OUT = ( W_IN + H_IN ) / 1.8;
  int H_OUT = ( W_IN + H_IN ) / 1.8;
      double GeoData[6] ;
  CGDALRaster  rasterOut;
    rasterOut.CreateGeoTIFF( name_out, W_OUT, H_OUT, 
                                GeoData, rasterIn.GetProjectionData() )  ;
  bool isNew = false ;
  if(rasterOut.isNull()) {
    isNew = true ;
    double GeoData[6] ;
            puts("ya tut GetGeoTransform(GeoData)");
    rasterIn.GetGeoTransform(GeoData) ;
            puts("ya tut rasterOut.CreateGeoTIFF(");
    if( !rasterOut.CreateGeoTIFF( name_out, W_OUT, H_OUT, 
                                GeoData, rasterIn.GetProjectionData() ) ) printf("Gdal fail") ;
  } ;

  QMatrix M_IN = rasterIn.GetTransformMatrix() ;
//размеры блока по умолчанию
  int BlockSizeX_OUT = 1024 ; //l
  int BlockSizeY_OUT = 1024 ;
  int BlockSizeX_IN = 1448 ;//ceil( BlockSizeX_OUT * sqrt(2) );
  int BlockSizeY_IN = 1448 ;//L
  int W,H;
  int delta_l = 212 ; //dl
//  ____
//   | /\l|  <- quadrat учтено максимальное искажение
//   |/  \|          __
//   |\  /|L   L=l*\/2  
//   |_\/_|    dl=(L-l)/2

    // кол-во блоков
  int NumBlocksX_OUT = (int) ceil( double(W_OUT) / double(BlockSizeX_OUT) ) ;
  int NumBlocksY_OUT = (int) ceil( double(H_OUT) / double(BlockSizeY_OUT) ) ;
  int NumBlocksX_IN  = (int) ceil( double(W_IN ) / double(BlockSizeX_IN ) ) ;
  int NumBlocksY_IN  = (int) ceil( double(H_IN ) / double(BlockSizeY_IN ) ) ;  
  //рассчет размеров блока (N_block*h_block = H_raster) IN
  if ( BlockSizeX_IN> W_IN )
    BlockSizeX_IN= W_IN;
  else
    BlockSizeX_IN= W_IN / NumBlocksX_IN;
  if ( BlockSizeY_IN> H_IN )
    BlockSizeY_IN= H_IN;
  else
    BlockSizeY_IN= H_IN / NumBlocksY_IN;
  //рассчет размеров блока (N_block*h_block = H_raster) OUT
    if ( BlockSizeX_OUT> W_OUT)
    BlockSizeX_OUT= W_OUT;
  else
    BlockSizeX_OUT= W_OUT/ NumBlocksX_OUT;
  if ( BlockSizeY_OUT> H_OUT)
    BlockSizeY_OUT= H_IN;
  else
    BlockSizeY_OUT= H_IN / NumBlocksY_OUT;

  QVector3D ro[4];
  QVector3D e1,e2,b,rc;
  QVector3D OA, OO2,O2A, OC, CA, OAn, OO2n,O2An, OCn, CAn;
  QVector3D OC2, C2A, OC2N, C2AN;  

  int r,c;
  double r0,c0;

  r0=H_OUT/2.0;
  c0=W_OUT/2.0;

  ro[0]=QVector3D(YD.x(),YD.y(),0);   //!!координаты точек->QVector3D
  ro[1]=QVector3D(YU.x(),YU.y(),0);
  ro[2]=QVector3D(XL.x(),XL.y(),0);
  ro[3]=QVector3D(XR.x(),XR.y(),0);      
  
  e1=ro[3]-ro[2];
  e2=ro[1]-ro[0];

  e1=e1.normalized ();
  e2=e2.normalized ();
  b=ro[0]-ro[2];

  double e11,e12,e22,e1b,e2b;
  double D,D1,t1;

  e11=QVector3D::dotProduct(e1,e1);
  e12=QVector3D::dotProduct(e1,e2);
  e22=QVector3D::dotProduct(e2,e2);
  e1b=QVector3D::dotProduct(e1,b);
  e2b=QVector3D::dotProduct(e2,b);

  D=-e11*e22+e12*e12;  //преобразование
  D1=-e1b*e22+e2b*e12;

  t1=D1/D;

  printf("t1=%lf\n",t1);
  
  rc=ro[2]+e1*t1;

  printf("cc=%lf rc=%lf\n",rc.x(),rc.y());
//  getchar();       


  for( int iBlock = 0 ; iBlock < NumBlocksY_OUT; iBlock ++ ) {
    for( int jBlock = 0 ; jBlock < NumBlocksX_OUT; jBlock ++ ) {
  //очередные координаты блока
     QPoint ijPos(jBlock*BlockSizeX_OUT, iBlock*BlockSizeY_OUT) ;
     
  // выставлЯем очередные размеры блока
      int BlockWidth = BlockSizeX_OUT;
      int BlockHeight = BlockSizeY_OUT;

      //if( ijPos.x() + BlockWidth > W1 ) continue ;
      //if( ijPos.y() + BlockHeight > H1 ) continue ;

      if( ijPos.x() + BlockWidth > W_OUT) BlockWidth = W_OUT - ijPos.x() ;
      if( ijPos.y() + BlockHeight > H_OUT) BlockHeight = H_OUT - ijPos.y() ;

      // читаем блок результирующей карты
      QImage ijBlock_OUT ;
      if(isNew) {
        ijBlock_OUT= QImage( BlockWidth, BlockHeight, QImage::Format_RGB32 ) ;
        ijBlock_OUT.fill(0) ;
      }
      else {
        puts("ya tut");
        ijBlock_OUT= rasterOut.GetQImage(ijPos, BlockWidth, BlockHeight) ;
        }

      fprintf(stdout,"block: iBlock = %d : jBlock = %d\n", iBlock, jBlock) ;
      // читаем блок исходного растра
      QImage ijBlock_IN ;
         QPoint ijPos_IN;

//возможные случаи извлечениa с выходом за границу
//123
//894
//765

      if(iBlock==0&&jBlock==0) {  //1
          ijPos_IN = ijPos ;
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN - delta_l;
      } 
      if(iBlock==0&&jBlock!=0&&jBlock!=NumBlocksX_OUT) { //2
      //    ijPos_IN = ijPos - QPoint( (BlockSizeX_IN - BlockSizeX_OUT) / 2 , 0 ) ;
          ijPos_IN = ijPos - QPoint(delta_l,0);
          W = BlockSizeX_IN;
          H = BlockSizeY_IN - delta_l;
      }
      if(iBlock==0&&jBlock==NumBlocksX_OUT) { //3
     //     ijPos_IN = ijPos - QPoint( (BlockSizeX_IN - BlockSizeX_OUT) / 2 , 0 ) ;
          ijPos_IN = ijPos - QPoint(delta_l,0);
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN - delta_l;     
      }
      if(jBlock==NumBlocksX_OUT&&iBlock!=0&&iBlock!=NumBlocksY_OUT) { //4
          ijPos_IN = ijPos - QPoint(delta_l,delta_l);
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN;              
      }
      if(jBlock==NumBlocksX_OUT&&iBlock==NumBlocksY_OUT) { //5
          ijPos_IN = ijPos - QPoint(delta_l,delta_l);
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN - delta_l;        
      }      
      if(iBlock=NumBlocksY_OUT&&jBlock!=0&&jBlock!=NumBlocksX_OUT) { //6
      //    ijPos_IN = ijPos - QPoint( 0 , (BlockSizeY_IN - BlockSizeY_OUT) / 2 ) ;
          ijPos_IN = ijPos - QPoint (delta_l,delta_l);
          W = BlockSizeX_IN;
          H = BlockSizeY_IN - delta_l;      
      } 
      if(iBlock=NumBlocksY_OUT&&jBlock==0) { //7
      //    ijPos_IN = ijPos - QPoint( 0 , (BlockSizeY_IN - BlockSizeY_OUT) / 2 ) ;
          ijPos_IN = ijPos - QPoint (0,delta_l);
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN - delta_l;      
      }      
      if(iBlock!=0&&iBlock!=NumBlocksY_OUT&&jBlock==0) { //8
      //    ijPos_IN = ijPos - QPoint( 0 , (BlockSizeY_IN - BlockSizeY_OUT) / 2 ) ;
          ijPos_IN = ijPos - QPoint (0,delta_l);
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN;      
      }
      if(iBlock!=0&&jBlock!=0&&iBlock!=NumBlocksY_OUT&&jBlock!=NumBlocksX_OUT) { //9
     //     ijPos_IN = ijPos - QPoint((BlockSizeX_IN - BlockSizeX_OUT) / 2,(BlockSizeY_IN - BlockSizeY_OUT) / 2);
          ijPos_IN = ijPos - QPoint (delta_l,delta_l);
          W = BlockSizeX_IN;
          H = BlockSizeY_IN;     
      }          
      ijBlock_IN= rasterIn.GetQImage(ijPos_IN, W, H ) ;
      
      
//  for(r=1;r<H_OUT;r++) {
  // for(c=1;c<W_OUT;c++) {
  for(r=1;r<BlockSizeY_OUT;r++) {
   for(c=1;c<BlockSizeX_OUT;c++) {    
      
        double cp,rp;
        double dc,dr;
        QVector3D point;
       
        
        dc=c-c0;
        dr=r0-r;
    
        point=rc+e1*dc+e2*dr; //координаты во входном снимке
        cp=point.x();
        rp=point.y();
    
        //if ((cp>=d)&&(rp>=d)&&(cp<im.width()-d)&&(rp<im.height()-d)) {
        uint color;
        QRgb colRgb;
         if( cp < W_IN && cp >= 0 ) {
            if( rp < H_IN && rp >= 0 ) 
              colRgb = ijBlock_IN.pixel(cp,rp);
            //  colRgb=qRgb(255,255,255);
            else
              colRgb = qRgb(0,0,0);
            }
         else
            colRgb = qRgb(0,0,0);
         
         ijBlock_OUT.setPixel(c,r,colRgb);
    }
  }
   rasterOut.Write( ijPos, ijBlock_OUT) ;
      }
    }
}
*/

/*void KrestWindow::pressed_button_run() {
   calculCoordKrest();
//---------------ALhorithm---------------------------------
 //QString name_in = nameIn;
// QString name_out = nameIn+"out.bmp";
// QString name_in = "E:/Shtanov/images/img_doc_1.bmp";
 //QString name_out = "E:/Shtanov/images/img_doc_1_out_1.bmp";
// nameIn = name_in; // put nameIn in class
   QString name_in = nameIn;
   QString name_out = QObject::tr("%1_out.bmp").arg(name_in);
 CGDALRaster rasterIn(nameIn) ;
  int W_IN = rasterIn.Width() ;
  int H_IN = rasterIn.Height() ;
//  int W_OUT = (int)(double)( W_IN + H_IN ) / 1.414;
//  int H_OUT = (int)(double)( W_IN + H_IN ) / 1.414;
  int W_OUT = W_IN;
  int H_OUT = H_IN;
  int delta_W = (W_OUT-W_IN)/2;
  int delta_H = (H_OUT-H_IN)/2;  
  double GeoData[6] ;
  CGDALRaster  rasterOut;
    rasterOut.CreateGeoTIFF( name_out, W_OUT, H_OUT, 
                                GeoData, rasterIn.GetProjectionData() )  ;
  bool isNew = false ;
  if(rasterOut.isNull()) {
    isNew = true ;
    double GeoData[6] ;
            puts("ya tut GetGeoTransform(GeoData)");
    rasterIn.GetGeoTransform(GeoData) ;
            puts("ya tut rasterOut.CreateGeoTIFF");
    if( !rasterOut.CreateGeoTIFF( name_out, W_OUT, H_OUT, 
                                GeoData, rasterIn.GetProjectionData() ) ) printf("Gdal fail") ;
  } ;

//  QMatrix M_IN = rasterIn.GetTransformMatrix() ;
//размеры блока по умолчанию
  int BlockSizeX_OUT = 1280 ; //l
  int BlockSizeY_OUT = 1280 ;
  int BlockSizeX_IN = 1380 ;//ceil( BlockSizeX_OUT * sqrt(2) );
  int BlockSizeY_IN = 1380 ;//L
  int W,H;
  int delta_l = (BlockSizeX_IN - BlockSizeX_OUT)/2; //dl
//    ____
//   | /\l|  <- квадрат учтено максимальное искажение
//   |/  \|           __
//   |\  /|L   L =l*\/2  
//   |_\/_|    dl=(L-l)/2

    // кол-во блоков
  int NumBlocksX_OUT = (int) ceil( double(W_OUT) / double(BlockSizeX_OUT) ) ;
  int NumBlocksY_OUT = (int) ceil( double(H_OUT) / double(BlockSizeY_OUT) ) ;
  int NumBlocksX_IN  = (int) ceil( double(W_IN ) / double(BlockSizeX_IN ) ) ;
  int NumBlocksY_IN  = (int) ceil( double(H_IN ) / double(BlockSizeY_IN ) ) ;  
  //рассчет размеров блока (N_block*h_block = H_raster) IN
  if ( BlockSizeX_IN > W_IN )
    BlockSizeX_IN= W_IN;
  else
    BlockSizeX_IN=(int)(double) W_IN /(double) NumBlocksX_IN;
  if ( BlockSizeY_IN > H_IN )
    BlockSizeY_IN= H_IN;
  else
    BlockSizeY_IN=(int)(double) H_IN /(double) NumBlocksY_IN;
  //рассчет размеров блока (N_block*h_block = H_raster) OUT
  if ( BlockSizeX_OUT> W_OUT)
    BlockSizeX_OUT= W_OUT;
  else
    BlockSizeX_OUT= (int)(double)W_OUT /(double) NumBlocksX_OUT;
  if ( BlockSizeY_OUT> H_OUT)
    BlockSizeY_OUT= H_IN;
  else
    BlockSizeY_OUT= (int)(double)H_IN /(double) NumBlocksY_OUT;

  printf("HIN %d WIN %d HOUT %d WOUT %d BXIN%d BYIN%d BXOUT %d BYOUT %d\n",H_IN,W_IN,H_OUT,W_OUT,BlockSizeX_IN,BlockSizeY_IN,BlockSizeX_OUT,BlockSizeY_OUT);

  QVector3D ro[4], ro_block[4];
  QVector3D e1,e2,b,rc, e1_block,e2_block,b_block,rc_block;
  QVector3D OA, OO2,O2A, OC, CA, OAn, OO2n,O2An, OCn, CAn;
  QVector3D OC2, C2A, OC2N, C2AN;  

  int r,c, r_block,c_block;
  double r0,c0,r0_IN,c0_IN,c0b_OUT,r0b_OUT;//, r0_block,c0_block;

  r0=H_OUT/2.0;  //координаты центра выходного снимка
  c0=W_OUT/2.0;

  //r0_block=BlockSizeY_OUT/2.0;  //координаты центра выходного снимка
 // c0_block=BlockSizeX_OUT/2.0;

  r0b_OUT = BlockSizeY_OUT/2.0;
  c0b_OUT = BlockSizeX_OUT/2.0;

 // r0_IN = H_IN / 2.0; //координаты центра входного снимка
  //c0_IN = W_IN / 2.0;

  ro[0]=QVector3D(YD.x(),YD.y(),0);   //!!координаты крестов->QVector3D
  ro[1]=QVector3D(YU.x(),YU.y(),0);
  ro[2]=QVector3D(XL.x(),XL.y(),0);
  ro[3]=QVector3D(XR.x(),XR.y(),0);    

 // ro_block[0]=QVector3D(YD.x(),YD.y(),0);   //!!координаты крестов->QVector3D
//  ro_block[1]=QVector3D(YU.x(),YU.y(),0);
 // ro_block[2]=QVector3D(XL.x(),XL.y(),0);
 // ro_block[3]=QVector3D(XR.x(),XR.y(),0);  
  
  e1=ro[3]-ro[2];  //направленийа осей координат ГеоСК
  e2=ro[1]-ro[0];  //во входном снимке

 // e1_block=ro_block[3]-ro_block[2];  //направленийа осей координат ГеоСК
  //e2_block=ro_block[1]-ro_block[0];  //во входном снимке
  
  e1=e1.normalized (); //нормированный базис ГеоСК входного снимка
  e2=e2.normalized ();

  //e1_block=e1_block.normalized (); //нормированный базис ГеоСК входного снимка
  //e2_block=e2_block.normalized ();
  
  b =ro[0]-ro[2];      //

 // b_block =ro_block[0]-ro_block[2];      //

  double e11,e12,e22,e1b,e2b;
  double D,D1,t1;

  double e11_block,e12_block,e22_block,e1b_block,e2b_block;
  double D_block,D1_block,t1_block;

  e11=QVector3D::dotProduct(e1,e1); //скалaрные произведениа
  e12=QVector3D::dotProduct(e1,e2);
  e22=QVector3D::dotProduct(e2,e2);
  e1b=QVector3D::dotProduct(e1,b);
  e2b=QVector3D::dotProduct(e2,b);

 // e11_block=QVector3D::dotProduct(e1_block,e1_block); //скалaрные произведениа
 // e12_block=QVector3D::dotProduct(e1_block,e2_block);
 // e22_block=QVector3D::dotProduct(e2_block,e2_block);
 // e1b_block=QVector3D::dotProduct(e1_block,b_block);
 // e2b_block=QVector3D::dotProduct(e2_block,b_block);

  D =-e11*e22+e12*e12;  //преобразование
  D1=-e1b*e22+e2b*e12;

  t1=D1/D;

  printf("t1=%lf\n",t1);
  
  rc=ro[2]+e1*t1;

//  D_block =-e11_block*e22_block+e12_block*e12_block;  //преобразование
 // D1_block=-e1b_block*e22_block+e2b_block*e12_block;

 // t1_block=D1_block/D_block;

 // printf("t1=%lf\n",t1);
  
//  rc_block=ro_block[2]+e1_block*t1_block;

 // printf("cc=%lf rc=%lf\n",rc.x(),rc.y());
//  getchar();       
  
  for( int iBlock = 0 ; iBlock < NumBlocksY_OUT; iBlock ++ ) {
    for( int jBlock = 0 ; jBlock < NumBlocksX_OUT; jBlock ++ ) {
  //очередные координаты блока
     QPoint ijPos(jBlock*BlockSizeX_OUT, iBlock*BlockSizeY_OUT) ;
     
  // выставлЯем очередные размеры блока
      int BlockWidth = BlockSizeX_OUT;
      int BlockHeight = BlockSizeY_OUT;

      //if( ijPos.x() + BlockWidth > W1 ) continue ;
      //if( ijPos.y() + BlockHeight > H1 ) continue ;

   //   if( ijPos.x() + BlockWidth  > W_OUT) BlockWidth  = W_OUT - ijPos.x( ) ;
   //   if( ijPos.y() + BlockHeight > H_OUT) BlockHeight = H_OUT - ijPos.y( ) ;

      // читаем блок результирующей карты
      QImage ijBlock_OUT ;
      if(isNew) {
        ijBlock_OUT = QImage( BlockWidth, BlockHeight, QImage::Format_RGB32 ) ;
        ijBlock_OUT.fill(0) ;
      }
      else {
        ijBlock_OUT= rasterOut.GetQImage(ijPos, BlockWidth, BlockHeight) ;
        }

      fprintf(stdout,"block: iBlock = %d : jBlock = %d\n", iBlock, jBlock) ;
      // читаем блок исходного растра
      QImage ijBlock_IN ;
         QPoint ijPos_IN ;
         
        double cp,rp;
        double dc,dr;
        QVector3D point;
        ijPos += QPoint(BlockSizeX_OUT/2,BlockSizeY_OUT/2);
        
        dc = ijPos.x()-c0; //координаты в IM_OUT в ГЕО_СК
        dr = r0-ijPos.y();
    
        point=rc+e1*dc+e2*dr; //координаты во входном снимке в СК растра
       // cp=point.x()-c0_IN; 
       // rp=r0_IN-point.y();
        ijPos_IN = point.toPoint();
        
// 123   //возможные случаи извлечениa фрагмента с выходом за границу растра
// 894  
// 765
        if( (ijPos_IN.x())<BlockSizeX_IN/2 
          &&(ijPos_IN.y())<BlockSizeY_IN/2) {//1
              W=(ijPos_IN.x()+BlockSizeX_IN/2);
              H=(ijPos_IN.y()+BlockSizeY_IN/2);
#if DEBUG
              printf("1:OUTX %d OUTY %d",ijPos.x(),ijPos.y());
              printf("W %d H %d INX %d INY %d",W,H,ijPos_IN.x(),ijPos_IN.y());
              getchar();
#endif              
              ijPos_IN=QPoint(0,0);//ipen(ijPos_IN);

        }
      //  if( (ijPos_IN.x()<BlockSizeX_IN/2) && ijPos_IN.x()>0
        //  &&(ijPos_IN.y()<BlockSizeY_IN/2)&& ijPos_IN.y()>0) {//1
        if( (ijPos_IN.x()>BlockSizeX_IN/2)
          &&(ijPos_IN.x()+BlockSizeX_IN/2)<W_IN
          &&(ijPos_IN.y()<BlockSizeY_IN/2)) {//2    ok          
              W=BlockSizeX_IN;
              H=(ijPos_IN.y()+BlockSizeY_IN/2);
#if DEBUG
              printf("2:OUTX %d OUTY %d",ijPos.x(),ijPos.y());
              printf("W %d H %d INX %d INY %d",W,H,ijPos_IN.x(),ijPos_IN.y());
              getchar();
#endif
              ijPos_IN=QPoint(ijPos_IN.x()-BlockSizeX_IN/2,0);//.setY(0);//ipen(ijPos_IN);//QPoint(ijPos_IN.x()-BlockSizeX_IN/2,0);
        }      
        else if( (ijPos_IN.x()+BlockSizeX_IN/2)>W_IN
          &&(ijPos_IN.y()<BlockSizeY_IN/2)) {//3              
              W=BlockSizeX_IN/2+W_IN-ijPos_IN.x();
              H=BlockSizeY_IN/2+ijPos_IN.y();
#if DEBUG
              printf("3:OUTX %d OUTY %d",ijPos.x(),ijPos.y());
              printf("W %d H %d INX %d INY %d",W,H,ijPos_IN.x(),ijPos_IN.y());
              getchar();
#endif
              ijPos_IN=QPoint(ijPos_IN.x()-BlockSizeX_IN/2,0);              
        }  
        else if( (ijPos_IN.x()+BlockSizeX_IN/2)>W_IN
          &&(ijPos_IN.y()>BlockSizeY_IN/2)
          &&(ijPos_IN.y()+BlockSizeY_IN/2)<H_IN) {//4             
              W=BlockSizeX_IN/2+W_IN-ijPos_IN.x();
              H=BlockSizeY_IN;
#if DEBUG
              printf("4:OUTX %d OUTY %d",ijPos.x(),ijPos.y());
              printf("W %d H %d INX %d INY %d",W,H,ijPos_IN.x(),ijPos_IN.y());
              getchar();
#endif             
              ijPos_IN=QPoint(ijPos_IN.x()-BlockSizeX_IN/2,ijPos_IN.y()-BlockSizeY_IN/2);
        }     
        else if( (ijPos_IN.x()+BlockSizeX_IN/2)>W_IN
          &&(ijPos_IN.y()+BlockSizeY_IN/2)>H_IN) {//5
              W=BlockSizeX_IN/2+W_IN-ijPos_IN.x();
              H=BlockSizeY_IN/2+H_IN-ijPos_IN.y();
#if DEBUG
              printf("5:OUTX %d OUTY %d",ijPos.x(),ijPos.y());
              printf("W %d H %d INX %d INY %d",W,H,ijPos_IN.x(),ijPos_IN.y());
              getchar();
#endif            
              ijPos_IN=QPoint(ijPos_IN.x()-BlockSizeX_IN/2,ijPos_IN.y()-BlockSizeY_IN/2);              
        }    
        else if( (ijPos_IN.x()>BlockSizeX_IN/2)
          &&(ijPos_IN.x()+BlockSizeX_IN/2)<W_IN
          &&(ijPos_IN.y()+BlockSizeY_IN/2)>H_IN) {//6
              W=BlockSizeX_IN;
              H=BlockSizeY_IN/2+H_IN-ijPos_IN.y();
#if DEBUG
              printf("6:OUTX %d OUTY %d",ijPos.x(),ijPos.y());
              printf("W %d H %d INX %d INY %d",W,H,ijPos_IN.x(),ijPos_IN.y());
              getchar();
#endif             
              ijPos_IN=QPoint(ijPos_IN.x()-BlockSizeX_IN/2,ijPos_IN.y()-BlockSizeY_IN/2);              
        }   
        else if( (ijPos_IN.x()<BlockSizeX_IN/2)
          &&(ijPos_IN.y()+BlockSizeY_IN/2)>H_IN) {//7
              W=(ijPos_IN.x()+BlockSizeX_IN/2);
              H=BlockSizeY_IN/2+H_IN-ijPos_IN.y();
#if DEBUG
              printf("7:OUTX %d OUTY %d",ijPos.x(),ijPos.y());
              printf("W %d H %d INX %d INY %d",W,H,ijPos_IN.x(),ijPos_IN.y());
              getchar();
#endif           
              ijPos_IN=QPoint(0,ijPos_IN.y()-BlockSizeY_IN/2);              
        }   
        else if( (ijPos_IN.x()<BlockSizeX_IN/2)
          &&(ijPos_IN.y()>BlockSizeY_IN/2)
          &&(ijPos_IN.y()+BlockSizeY_IN/2)<H_IN) {//8
              W=BlockSizeX_IN/2+ijPos_IN.x();
              H=BlockSizeY_IN;
#if DEBUG
              printf("8:OUTX %d OUTY %d",ijPos.x(),ijPos.y());
              printf("W %d H %d INX %d INY %d",W,H,ijPos_IN.x(),ijPos_IN.y());
              getchar();
#endif             
              ijPos_IN=QPoint(0,ijPos_IN.y()-BlockSizeY_IN/2);              
        }  
        else if( (ijPos_IN.x()+BlockSizeX_IN/2)<W_IN
          &&(ijPos_IN.x()>BlockSizeX_IN/2)
          &&(ijPos_IN.y()>BlockSizeY_IN/2)
          &&(ijPos_IN.y()+BlockSizeY_IN/2)<H_IN) {//9
              W=BlockSizeX_IN;
              H=BlockSizeY_IN;
#if DEBUG
              printf("9:OUTX %d OUTY %d",ijPos.x(),ijPos.y());
              printf("W %d H %d INX %d INY %d",W,H,ijPos_IN.x(),ijPos_IN.y());
              getchar();
#endif             
              ijPos_IN=QPoint(ijPos_IN.x()-BlockSizeX_IN/2,ijPos_IN.y()-BlockSizeY_IN/2);              
        }        
    /*  if(iBlock==0&&jBlock==0) {  //1
          ijPos_IN = ijPos ;
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN - delta_l;
      } 
      if(iBlock==0&&jBlock!=0&&jBlock!=NumBlocksX_OUT) { //2
      //    ijPos_IN = ijPos - QPoint( (BlockSizeX_IN - BlockSizeX_OUT) / 2 , 0 ) ;
          ijPos_IN = ijPos - QPoint(delta_l,0);
          W = BlockSizeX_IN;
          H = BlockSizeY_IN - delta_l;
      }
      if(iBlock==0&&jBlock==NumBlocksX_OUT) { //3
     //     ijPos_IN = ijPos - QPoint( (BlockSizeX_IN - BlockSizeX_OUT) / 2 , 0 ) ;
          ijPos_IN = ijPos - QPoint(delta_l,0);
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN - delta_l;     
      }
      if(jBlock==NumBlocksX_OUT&&iBlock!=0&&iBlock!=NumBlocksY_OUT) { //4
          ijPos_IN = ijPos - QPoint(delta_l,delta_l);
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN;              
      }
      if(jBlock==NumBlocksX_OUT&&iBlock==NumBlocksY_OUT) { //5
          ijPos_IN = ijPos - QPoint(delta_l,delta_l);
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN - delta_l;        
      }      
      if(iBlock=NumBlocksY_OUT&&jBlock!=0&&jBlock!=NumBlocksX_OUT) { //6
      //    ijPos_IN = ijPos - QPoint( 0 , (BlockSizeY_IN - BlockSizeY_OUT) / 2 ) ;
          ijPos_IN = ijPos - QPoint (delta_l,delta_l);
          W = BlockSizeX_IN;
          H = BlockSizeY_IN - delta_l;      
      } 
      if(iBlock=NumBlocksY_OUT&&jBlock==0) { //7
      //    ijPos_IN = ijPos - QPoint( 0 , (BlockSizeY_IN - BlockSizeY_OUT) / 2 ) ;
          ijPos_IN = ijPos - QPoint (0,delta_l);
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN - delta_l;      
      }      
      if(iBlock!=0&&iBlock!=NumBlocksY_OUT&&jBlock==0) { //8
      //    ijPos_IN = ijPos - QPoint( 0 , (BlockSizeY_IN - BlockSizeY_OUT) / 2 ) ;
          ijPos_IN = ijPos - QPoint (0,delta_l);
          W = BlockSizeX_IN - delta_l;
          H = BlockSizeY_IN;      
      }
      if(iBlock!=0&&jBlock!=0&&iBlock!=NumBlocksY_OUT&&jBlock!=NumBlocksX_OUT) { //9
     //     ijPos_IN = ijPos - QPoint((BlockSizeX_IN - BlockSizeX_OUT) / 2,(BlockSizeY_IN - BlockSizeY_OUT) / 2);
          ijPos_IN = ijPos - QPoint (delta_l,delta_l);
          W = BlockSizeX_IN;
          H = BlockSizeY_IN;     
      }          
      //ijPos_IN -= QPoint(BlockSizeX_IN,BlockSizeY_IN);
#if DEBUG
              printf("%d %d %d %d",W,H,ijPos_IN.x(),ijPos_IN.y());
              getchar();
#endif     
      ijBlock_IN = rasterIn.GetQImage(ijPos_IN, W, H ) ;
      
      int WB=BlockSizeX_IN;
      int HB=BlockSizeY_IN;
      QPoint YDB((int)(double)YD.x()*(double)WB/(double)W_IN,(int)(double)YD.y()*(double)HB/(double)H_IN);
      QPoint YUB((int)(double)YU.x()*(double)WB/(double)W_IN,(int)(double)YU.y()*(double)HB/(double)H_IN);      
      QPoint XLB((int)(double)XL.x()*(double)WB/(double)W_IN,(int)(double)XL.y()*(double)HB/(double)H_IN);      
      QPoint XRB((int)(double)XR.x()*(double)WB/(double)W_IN,(int)(double)XR.y()*(double)HB/(double)H_IN);      
      Block_transform block_tr(YDB,YUB,XLB,XRB,BlockSizeX_OUT, BlockSizeY_OUT);
//      ijBlock_IN = rasterIn.GetQImage(ijPos, BlockSizeX_OUT, BlockSizeY_OUT) ;
    QString name_test = QObject::tr("E:/Shtanov/images/test/img_%1_%2.bmp").arg(iBlock).arg(jBlock);
      ijBlock_IN.save(name_test);
  //  QString name_testOUT = QObject::tr("E:/Shtanov/images/test/img_%1_%2.bmp").arg(iBlock).arg(jBlock);      
   //   ijBlock_IN.save(name_test);      
  //    ijBlock_OUT = ijBlock_IN;
      
//  for(r=1;r<H_OUT;r++) {
  // for(c=1;c<W_OUT;c++) {
  for(r=0;r<BlockSizeY_OUT;r++) {
   for(c=0;c<BlockSizeX_OUT;c++) {    
      
        double cp,rp;
        double dc,dr;
        QVector3D point;       
        
        dc=c-block_tr.c0b_OUT;
        dr=block_tr.r0b_OUT-r;
    
        point=block_tr.rc_block+block_tr.e1_block*dc+block_tr.e2_block*dr; //координаты во входном снимке
        cp=point.x();
        rp=point.y();
        //cp -= BlockSizeX_IN*jBlock;
        //rp -= BlockSizeY_IN*iBlock;        
       
        //if ((cp>=d)&&(rp>=d)&&(cp<im.width()-d)&&(rp<im.height()-d)) {
        uint color;
       // QRgb colRgb;
   //      if( cp < BlockSizeX_IN && cp >= 0 ) {
     //       if( rp < BlockSizeY_IN && rp >= 0 )
         if( cp < W && cp >= 0 ) {
            if( rp < H && rp >= 0 )              
              color = qGray(ijBlock_IN.pixel(cp,rp));
            //  colRgb=qRgb(255,255,255);
            else
              color = qGray(255,255,255);
            }
         else
            color = qGray(0,0,0);
         
         ijBlock_OUT.setPixel(c,r,color);
    }
  }
   ijPos -= QPoint( BlockSizeX_OUT/2 , BlockSizeY_OUT/2 );
   rasterOut.Write( ijPos, ijBlock_OUT );
      }
    }
}*/


/*void KrestWindow::pressed_button_run() {
   calculCoordKrest();
//---------------ALhorithm---------------------------------
   QString name_in = nameIn;
   QString name_out = nameOut;//QObject::tr("%1_out.tiff").arg(name_in);
   QRegExp tiff_regexp ("(.tiff|.bmp|.jpg|.img|.png|.gif|.tif)");
   CGDALRaster rasterIn;
 //  if(tiff_regexp.search(nameIn)!=-1){
 //     rasterIn.set_path(nameIn) ;
 //     puts("rasterIn.set_path(nameIn) ;");
 //  }
 //  else {
  //    rasterIn.set_path(nameIn+".tiff") ;
 //     puts("rasterIn.set_path(nameIn+.tiff)");      
 //  }

  if(!rasterIn.Read(name_in)) {
     rasterIn.Read(name_in+".tiff");
      puts("rasterIn.Read(name_in+.tiff)");      
  }
  else 
      puts("rasterIn.Read(name_in)");    
  int W_IN = rasterIn.Width() ;
  int H_IN = rasterIn.Height() ;
  int W_OUT = W_IN;
  int H_OUT = H_IN;
  double GeoData[6] ;
  CGDALRaster  rasterOut;
  bool isNew = false ;

  QVector3D ro[4],roIN[4];
  QVector3D e1,e2,b,rc;
  QVector3D la1,la2,la;
  double r0,c0;

  ro[0]=QVector3D(YD.x(),YD.y(),0);   //!!координаты крестов->QVector3D
  ro[1]=QVector3D(YU.x(),YU.y(),0);
  ro[2]=QVector3D(XL.x(),XL.y(),0);
  ro[3]=QVector3D(XR.x(),XR.y(),0);   
  e1=ro[3]-ro[2];  //направленийа осей координат ГеоСК
  e2=ro[1]-ro[0];  //во входном снимке

  e1=e1.normalized (); //нормированный базис ГеоСК входного снимка
  e2=e2.normalized ();
  
  b =ro[0]-ro[2];      //

  double e11,e12,e22,e1b,e2b;
  double D,D1,t1;

  e11=QVector3D::dotProduct(e1,e1); //скалaрные произведениа
  e12=QVector3D::dotProduct(e1,e2);
  e22=QVector3D::dotProduct(e2,e2);
  e1b=QVector3D::dotProduct(e1,b);
  e2b=QVector3D::dotProduct(e2,b);

  D =-e11*e22+e12*e12;  //преобразование
  D1=-e1b*e22+e2b*e12;

  t1=D1/D;

  printf("t1=%lf\n",t1);
  
  rc=ro[2]+e1*t1;   

 /* if(cutting) {
     QVector3D coord_ramk[4],vect_ramk[4];
     QPointF coord_rez[4];
       coord_ramk[0] =QVector3D(RR->coord1.x(),RR->coord1.y(),0);
       coord_ramk[1] =QVector3D(RR->coord2.x(),RR->coord2.y(),0);      
       coord_ramk[2] =QVector3D(RR->coord3.x(),RR->coord3.y(),0);        
       coord_ramk[3] =QVector3D(RR->coord4.x(),RR->coord4.y(),0); 
          for(int l=0;l<4;++l) {
            vect_ramk[l]=rc-coord_ramk[l];
            coord_rez[l].setX(fabs(QVector3D::dotProduct(vect_ramk[l],e1)));
            coord_rez[l].setY(fabs(QVector3D::dotProduct(vect_ramk[l],e2)));            
          }
     double maxX,maxY;
      maxX=coord_rez[0].x( );
      maxY=coord_rez[0].y( );    
      for(int k=0;k<4;++k) {
        if(coord_rez[k].x( ) > maxX) {
             maxX = coord_rez[k].x( );
        }
        if(coord_rez[k].y( ) > maxY) {
             maxY = coord_rez[k].y( );
        }        
     }
     H_OUT =(int) ceil(2*maxY);
     W_OUT =(int) ceil(2*maxX);     
  } 


 // H_OUT = H_IN;
//  W_OUT = W_IN;
  if(!rasterOut.Read(name_out)) {
     rasterOut.CreateGeoTIFF( name_out, W_OUT, H_OUT, 
                                GeoData, rasterIn.GetProjectionData() );
      puts("rasterOut.CreateGeoTIFF( name_out,");      
  }
  else
      puts("rasterOut.Read(name_out)");      
    //размеры блока по умолчанию
  int BlockSizeX_OUT =  MAX_SIZE_BLOCK; //l
  int BlockSizeY_OUT =  MAX_SIZE_BLOCK;
//  int W,H;
    // кол-во блоков
  int NumBlocksX_OUT = (int) ceil( double(W_OUT) / double(BlockSizeX_OUT) ) ;
  int NumBlocksY_OUT = (int) ceil( double(H_OUT) / double(BlockSizeY_OUT) ) ;
  //рассчет размеров блока (N_block*h_block = H_raster) OUT
  if ( BlockSizeX_OUT >= W_OUT)
    BlockSizeX_OUT= W_OUT;
  else
    BlockSizeX_OUT= (int)((double)W_OUT /(double) NumBlocksX_OUT);
  if ( BlockSizeY_OUT >= H_OUT)
    BlockSizeY_OUT= H_OUT;
  else
    BlockSizeY_OUT= (int)((double)H_OUT /(double) NumBlocksY_OUT);

  printf("\nHIN %d WIN %d HOUT %d WOUT %d BXOUT %d BYOUT %d",H_IN,W_IN,H_OUT,W_OUT,BlockSizeX_OUT,BlockSizeY_OUT);
  printf("\nNumBlocksX_OUT %d NumBlocksY_OUT %d",NumBlocksX_OUT,NumBlocksY_OUT);
  r0=H_OUT/2.0;  //координаты центра выходного снимка
  c0=W_OUT/2.0;  

  double cp,rp;
  double dc,dr;  
  int icp,irp;
  QVector3D point;  
      QImage ijBlock_OUT;
      QImage ijBlock_IN;
  QPoint ijPosB ;
  
  for( int iBlock = 0 ; iBlock < NumBlocksY_OUT; iBlock ++ ) {
    for( int jBlock = 0 ; jBlock < NumBlocksX_OUT; jBlock ++ ) {
//очередные координаты блока
   QPoint ijPos(jBlock*BlockSizeX_OUT, iBlock*BlockSizeY_OUT) ; 
//читаем блок результирующей карты
   ijBlock_OUT= rasterOut.GetQImage(ijPos, BlockSizeX_OUT, BlockSizeY_OUT) ;
      
   QPoint pb[4];
   pb[0] = ijPos;
   pb[1] = ijPos+QPoint(BlockSizeX_OUT,0);               //12
   pb[2] = ijPos+QPoint(BlockSizeX_OUT,BlockSizeY_OUT);  //43 
   pb[3] = ijPos+QPoint(0,BlockSizeY_OUT); //координаты углов блока в выходном растре

  // rc = QVector3D(W_IN/2,H_IN/2,0);   
   
   QPointF pbIN[4];
   double dcIN[4],drIN[4];
     for( int k = 0; k < 4 ; ++k ) {
            dcIN[k]=pb[k].x()-c0;
            drIN[k]=r0-pb[k].y();
            point=rc+e1*dcIN[k]+e2*drIN[k];
            pbIN[k] = point.toPoint();     //координаты углов блока во входном растре
      }
     for( int k = 0; k < 4 ; ++k ) {
        if(pbIN[k].x()<0)pbIN[k].setX(0);
        if(pbIN[k].y()<0)pbIN[k].setY(0);  
        if(pbIN[k].x()>W_IN)pbIN[k].setX(W_IN);   
        if(pbIN[k].y()>H_IN)pbIN[k].setY(H_IN);        
      }
    double maxX,maxY,minX,minY;
    maxX=pbIN[0].x();
    maxY=pbIN[0].y();    
    minX=pbIN[0].x();    
    minY=pbIN[0].y();    
      for(int k=0;k<4;++k) {
        if(pbIN[k].x() > maxX) {
          if(pbIN[k].x() < W_IN)
             maxX = pbIN[k].x();
          else
             maxX = W_IN;
        }
        if(pbIN[k].y() > maxY) {
          if(pbIN[k].y()<H_IN)
             maxY = pbIN[k].y();   
          else
             maxY = H_IN;               
        }
        if(pbIN[k].x() < minX) {
          if(pbIN[k].x()>0)
             minX = pbIN[k].x();
          else
             minX = 0;
        }
        if(pbIN[k].y() < minY) {
          if(pbIN[k].y()>0)
             minY = pbIN[k].y();
          else
             minY = 0;
        }
      }
  int BlockSizeX_IN = ceil(maxX) - floor(minX); //l
  int BlockSizeY_IN = ceil(maxY) - floor(minY);
  QPoint ijPosIN(floor(minX),floor(minY));
 // printf("\nijPosXIN %d ijPosYIN %d",ijPosIN.x(),ijPosIN.y());
 // printf("\nmaxX %d minX %d maxY %d minY %d",maxX,minX,maxY,minY);
 // getchar();  
      fprintf(stdout,"block: iBlock = %d : jBlock = %d\n", iBlock, jBlock);
      // читаем блок исходного растра

        ijBlock_IN= rasterIn.GetQImage(ijPosIN, BlockSizeX_IN, BlockSizeY_IN);
      
       //QString name_test = QObject::tr("E:/Shtanov/images/test/img_%1_%2.bmp").arg(iBlock).arg(jBlock);
      //ijBlock_IN.save(name_test);
  //  QString name_testOUT = QObject::tr("E:/Shtanov/images/test/img_%1_%2.bmp").arg(iBlock).arg(jBlock);      
   //   ijBlock_IN.save(name_test); 

       
    for(int r=0;r<BlockSizeY_OUT;r++) {
       for(int c=0;c<BlockSizeX_OUT;c++) {         
//координаты точек блока внутри растра = координаты блока + координаты внутри блока
            ijPosB = ijPos + QPoint(c,r);
          //  printf("ijPosX %d ijPosY %d ijPosBX %d ijPosBY %d",ijPos.x(),ijPos.y(),ijPosB.x(),ijPosB.y());
  //        dc=c-c0;
   //         dr=r0-r;

            dc=ijPosB.x()-c0;
            dr=r0-ijPosB.y();


             
            point=rc+e1*dc+e2*dr; //координаты во входном снимке
            cp=point.x();
            rp=point.y();
            icp = (int)roundD(cp);
            irp = (int)roundD(rp);
          //  printf("\nCP %d RP %d ij_IN_X %d ij_IN_Y %d",icp,irp,ijPosIN.x(),ijPosIN.y());
           // getchar();
           // ijPos_IN = point.toPoint();
            //cp -= BlockSizeX_IN*jBlock;
            //rp -= BlockSizeY_IN*iBlock;        
           
            int color;
           // QRgb colRgb;
       //      if( cp < BlockSizeX_IN && cp >= 0 ) {
         //       if( rp < BlockSizeY_IN && rp >= 0 )
             if( icp < W_IN && icp >= 0 ) {
                if( irp < H_IN && irp >= 0 ) { 
                   if(icp-ijPosIN.x()<BlockSizeX_IN&&irp-ijPosIN.y()<BlockSizeY_IN)
                     color = qGray(ijBlock_IN.pixel(icp-ijPosIN.x(),irp-ijPosIN.y()));
                   else
                     color = qGray(ijBlock_IN.pixel(icp-ijPosIN.x()-1,irp-ijPosIN.y()-1));
                }
                else
                  color = qGray(0,0,0);
             }
             else
                color = qGray(0,0,0);
             
             ijBlock_OUT.setPixel(c,r,color);
        }
      }
      // ijPos -= QPoint( BlockSizeX_OUT/2 , BlockSizeY_OUT/2 );
       rasterOut.Write( ijPos, ijBlock_OUT );
        
      }
    }
  
}
*/

/*double MathRound(double num, int precision)
{
  AnsiString s, number = FloatToStr(num);
  bool sign = false;
  double k;
  int pos = number.Pos(DecimalSeparator);
  int len = number.Length();
  if ((pos == 0) || ((len - pos) <= precision)) return num;
  AnsiString prom = number.SubString(1, 1);
  if (prom == '-') sign = true;
  s = number.SubString(pos+precision+1, 1);
  int i = StrToInt(s);
  if (i >= 5){
   if (sign)
     k = floor(num*Power(10, precision))/Power(10, precision);
   else
     k = ceil(num*Power(10, precision))/Power(10, precision);
  }else{
   if (!sign)
     k = floor(num*Power(10, precision))/Power(10, precision);
   else
     k = ceil(num*Power(10, precision))/Power(10, precision);
  }
  return k;
}*/

/*         QPoint ijPos_IN ;
         
        double cp,rp;
        double dc,dr;
        QVector3D point;
        
        dc = ijPos.x()-c0; //координаты в IM_OUT в ГЕО_СК
        dr = r0-ijPos.y();
    
        point=rc+e1*dc+e2*dr; //координаты во входном снимке в СК растра
        ijPos_IN = point.toPoint();

        ijPixel_IN=  rasterIn.GetQImage(ijPos_IN, 1, 1) ; 
*/

/*void KrestWindow::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
  puts("op!");
if (event->button() != Qt::LeftButton)
         return;
printf("\nimyKrestType event: %d",imyKrestType);
     Krest *krest;

         if(imyKrestType) {
             krest = new Krest(QGI_Image);
   switch(imyKrestType) {
      case krestXL:
         krest->setText(tr("%1").arg("-X"));
         break;
      case krestXR:
         krest->setText(tr("%1").arg("+X"));
         break;
      case krestYU:
         krest->setText(tr("%1").arg("+Y"));
         break;
      case krestYD:
         krest->setText(tr("%1").arg("-Y"));
         break;         
    }
            // item->setBrush(myItemColor);
             scene->addItem(krest);
             krest->setPos(event->scenePos());
             krest->coord = krest->scenePos ();
    scene->clearSelection();
    krest->setSelected(true);
    imyKrestType = 0;
       //      emit setupKrest(krest,event->scenePos());
         }
}*/

/*void KrestWindow::mousePressEvent ( QGraphicsSceneMouseEvent * event )
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

void KrestWindow::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    if (rubberBandIsShown) {
        updateRubberBandRegion();
        rubberBandRect.setBottomRight(event->pos().toPoint ());
        updateRubberBandRegion();
    }
}
*/
/*void KrestWindow::mousePressEvent ( QMouseEvent * event )
{   
        printf("good1!");
   int corner = 0;
   if(cutting) {
   //   QRect rect(Margin, Margin,
       //          width() - 2 * Margin, height() - 2 * Margin);
     corner = RR->changed_corner(origin);
      if (event->button() == Qt::LeftButton&&corner) {
        //  if (rect.contains(event->pos())) {
              rubberBandIsShown = true;
                printf("good2!");
         //     rubberBandRect.setTopLeft(event->pos());
          //    rubberBandRect.setBottomRight(event->pos());

   // QRect rect1 = rubberBandRect.normalized();
    //QGraphicsRectItem *QGRI;
   // QGRI = new QGraphicsRectItem(rubberBandRect);
  //  scene->addItem(QGRI);              
  //  puts("fatal error!");
         //     updateRubberBandRegion();
    //puts("fatal error!");              
             //setCursor(Qt::CrossCursor);
     origin = event->pos();
     RR->change_coord(origin,corner);
 
    /* if (!rubberBand) {
         rubberBand = new QRubberBand(QRubberBand::Rectangle,graphicsView);//viewport());
         printf("%d %d",origin.x(),origin.y());
      }
     rubberBand->setGeometry(QRect(origin, QSize()));
     rubberBand->show();*/
     //scene->addItem(rubberBand);     
/*          }
      }
  
}*/

/*void KrestWindow::mouseMoveEvent ( QMouseEvent * event )
{
   int corner = 0;
    if (rubberBandIsShown) {
     corner = RR->changed_corner(event->pos());
      if(corner){
        RR->change_coord(event->pos(),corner);
        printf("good!");
        }
    //    updateRubberBandRegion();
     //   rubberBandRect.setBottomRight(event->pos());
     //   updateRubberBandRegion();
     
    }

  //   if (rubberBand)
   //      rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
    // QAbstractItemView::mouseMoveEvent(event);
}

void KrestWindow::updateRubberBandRegion()
{
  //  QRect rect = rubberBandRect.normalized();
 //   QGraphicsRectItem *QGRI;
 //   QGRI = new QGraphicsRectItem(rect);

    //scene->addItem(rect);
    graphicsView->update(QGRI->rect().left(), QGRI->rect().top(), QGRI->rect().width(), 1);
    //  puts("fatal error_in!");
    printf("\n%d %d %d",QGRI->rect().left(), QGRI->rect().top(), QGRI->rect().width());
    graphicsView->update(QGRI->rect().left(), QGRI->rect().top(), 1, QGRI->rect().height());
    graphicsView->update(QGRI->rect().left(), QGRI->rect().bottom(), QGRI->rect().width(), 1);
    graphicsView->update(QGRI->rect().right(), QGRI->rect().top(), 1, QGRI->rect().height());
   // scene->addItem(QGRI);    
}

void KrestWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) ) { //&& rubberBandIsShown) {
      //  rubberBandIsShown = false;
        cutting = false;
      //  updateRubberBandRegion();
    //    unsetCursor();
        if (rubberBand)
         rubberBand->hide();
       // viewport()->update();
        graphicsView->update();
        graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
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
}*/

/*void KrestWindow::wheelEvent(QWheelEvent *event)
{
    double numDegrees = -event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    //double factor = std::pow(1.125, numSteps);
    scale = std::pow(1.125, numSteps);
    graphicsView->scale(scale, scale);
    emit change_scale(scale); 
} */

/*void KrestWindow::addKrest()
{
  //  Krest *krest = new Krest;
//    krest->setText(tr("%1").arg(seqNumber + 1));
 //   setupKrest(krest);
}

void KrestWindow::setupKrest(Krest *krest)
{



}


//! отработка нажатия на кнопку мыши
void KrestWindow::mousePressEvent ( QMouseEvent * event )
{
 /*Qt::KeyboardModifiers modif;

 modif=event->modifiers();

 if (modif==Qt::NoModifier) {
  switch(event->button()) {
   case Qt::LeftButton:
    {
     // установка режима перемещения
     is_move=true;
     // запоминание положения точки где было нажатие
      pressed_left_button=event->pos();
     // установка курсора
     prev_curcor=this->cursor ();
     this->setCursor(Qt::ClosedHandCursor);
     
    };
    break;
   case Qt::RightButton:
    {
    };
    break;   
  };
 }
 else {
  prev_curcor=this->cursor ();
  this->setCursor(Qt::BlankCursor);
 };
}
//! отработка отпускания на кнопки мыши
void KrestWindow::mouseReleaseEvent ( QMouseEvent * event )
{
 /*Qt::KeyboardModifiers modif;

 modif=event->modifiers();

 if (is_move) {
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

  };
 }
 else {
  if (modif==Qt::NoModifier) {
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
  };  
 };

 if (Qt::BlankCursor==cursor ().shape ()) {
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

/*void KrestWindow::show_position_on_shot(double c,double r)
{
 QString str;

 str=QObject::tr("%1 %2").arg(int(c)).arg(int(r));
// str=QObject::tr("%1 %2").arg((QCursor::pos.x())).arg((QCursor::pos.y()));
// if (ns==1) {
//  lineEdit_X->setText(str);
 //};
// if (ns==2) {
//  lineEdit_Y->setText(str);
 //};
}*/        
//void KrestWindow::addKrest1()//,QPointF F)
//{
 //  printf("\nimyKrestType: %d",imyKrestType);
//   imyKrestType = type;
   // Krest *krest = new Krest(QGI_Image);
    //krest->setText(tr("%1").arg(seqNumber + 1));
  /*  switch(type) {
      case krestXL:
      //   krest->setText(tr("%1").arg("-X"));
         break;
      case krestXR:
     //    krest->setText(tr("%1").arg("+X"));
         break;
      case krestYU:
      //   krest->setText(tr("%1").arg("+Y"));
         break;
      case krestYD:
      //   krest->setText(tr("%1").arg("-Y"));
         break;         
    }*/
   // setupKrest(krest,F);
//}

/*void KrestWindow::updateCoord(QPointF a,QPointF b) {
  a=b;
  std::cout<<XR.x()<<" "<<XR.y()<<std::endl;
}*/
