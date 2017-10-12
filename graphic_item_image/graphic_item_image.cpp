#include "graphic_item_image.h"

//! конструктор
QGraphicsItem_Image::QGraphicsItem_Image(QGraphicsItem * parent ) : QGraphicsItem(parent)
{
  number_bands=0;
  //setFlag(QGraphicsItem::ItemIsMovable,true);
}
//! задание имени файла с растровыми данными
void QGraphicsItem_Image::set_image(const QString &name_file)
{
  prepareGeometryChange ();

  name=name_file;
  size_raster=QSize(0,0);
  number_bands=0;

  if (!name.isEmpty()) {
   GDALDataset  *pdata;
   pdata=(GDALDataset*)GDALOpen(name.toLocal8Bit().data(),GA_ReadOnly);
   if (pdata!=0) {
///-----проверка имеющихсЯ копий-----------------------------------------
    int numCount = (pdata->GetRasterBand(1))->GetOverviewCount(); 
    QFile ovrfile(name+".ovr");
       if(!ovrfile.exists()&&numCount==0){
           int anOverviewList[3] = { 4, 8, 16};
           pdata->BuildOverviews( "NEAREST", 3, anOverviewList, 0, NULL, 
                                      NULL, NULL );
       }
///----------------------------------------------------------------------
    int w,h;
    // чтение ширины и высоты растра в пикселях
    w=pdata->GetRasterXSize();
    h=pdata->GetRasterYSize();
    size_raster=QSize(w,h);
    // чтение количества bandов
    number_bands=pdata->GetRasterCount();
    //printf("number_bands=%d\n",number_bands);
    // формирование списка отображаемых слоев
      if (number_bands==1){
        list_bands[0]= 1;
      };
      if (number_bands>=3){
        list_bands[0]=3;
        list_bands[1]=2;
        list_bands[2]=1;        
      };
    // закрытие файла
    GDALClose(pdata);
   };    
  };
}

//! получение границ
QRectF QGraphicsItem_Image::boundingRect () const
{
  return QRectF(QPointF(0,0),size_raster) ;
}
//! отрисовка
void QGraphicsItem_Image::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
  QTransform transform;// параметры перехода от локальной СК к СК устройства отображения
  int W,H;
  QPaintDevice *pdevice;
  pdevice=painter->device () ;
  QRectF bound;

  bound=boundingRect ();
  if (pdevice!=0){
    W=pdevice->width ();
    H=pdevice->height ();

    // получение параметров перехода от локальной СК к СК устройства отображения
    transform=painter->combinedTransform();

    QRectF window_rect_in_LSC;// область устройства отображения в локальной СК
    QRectF workig_area_in_LSC;// область отображаемых данных в локальной СК
    QRectF workig_area_in_DSC;// область отображаемых данных в СК устройства отображения
    qreal xw1,xw2,yw1,yw2;// параметры области отображаемых растровых данных в СК окна
    qreal cf1,cf2,rf1,rf2;// параметры области отображаемых растровых данных в СК растра

    // вычисление прямоугольной области занимаемой устройством отобажения в локальной СК
    window_rect_in_LSC=transform.inverted().mapRect(QRectF(0,0,W,H));

    //printf("W=%d H=%d\n",W,H);
    // получение граничных точек прямоугольной области 
    window_rect_in_LSC.getCoords(&cf1, &rf1, &cf2, &rf2);
    
    //printf("1: cf1=%lf cf2=%lf rf1=%lf rf2=%lf\n",cf1,cf2,rf1,rf2);
    // определение области растровых данных требуемой для отображения
    workig_area_in_LSC=bound.intersected(QRectF(window_rect_in_LSC));
    
    // вычисление области отображаемых данных в СК устройства отображения
    workig_area_in_DSC=transform.mapRect(workig_area_in_LSC);

    // получение граничных точек прямоугольной области 
    workig_area_in_LSC.getCoords(&cf1, &rf1, &cf2, &rf2);
    workig_area_in_DSC.getCoords(&xw1, &yw1, &xw2, &yw2);

    //printf("2: cf1=%lf cf2=%lf rf1=%lf rf2=%lf\n",cf1,cf2,rf1,rf2);

    //printf("xw1=%lf xw2=%lf yw1=%lf yw2=%lf\n",xw1,xw2,yw1,yw2);
    // сохранение параметров объекта painter
    painter->save();
    // установка параметров перехода в единичное положение
    painter->resetTransform();
    if (!name.isEmpty()) {
    GDALDataset  *pdata;

      // открытие файла на чтение
    pdata=(GDALDataset*)GDALOpen(name.toLocal8Bit().data(),GA_ReadOnly);

    if (pdata!=0) {
     int nb_working;// количество используемых слоев
     QImage im;

     if ((!workig_area_in_DSC.isEmpty())&&((number_bands==1)||(number_bands>=3))) {
          
      int ww,hw;// размеры области отображения в пикселях в СК устройств отображения
      int bpp;// байт на пиксель в результирующем растре 
     
          // определение размеров отображаемой области в пикселях в СК устройств отображения 
          ww=xw2-xw1+1;
          hw=yw2-yw1+1;

      if (number_bands==1) {
            // обработка однослойного растра
       im=QImage(ww,hw,QImage::Format_Indexed8);
       bpp=1;
       nb_working=1;

            // формирование таблицы цветов
       int k;
       im.setNumColors(256);
       // чтение таблицы цветов
       GDALColorTable *pcolor_table;
       pcolor_table=(pdata->GetRasterBand(1))->GetColorTable ();
       
       for(k=0;k<256;k++) {
        if (pcolor_table!=0) {
         const GDALColorEntry *pcolor;
         pcolor=pcolor_table->GetColorEntry(k);
         im.setColor(k, QColor(pcolor->c1,pcolor->c2,pcolor->c3).rgb());
            // puts("135");
        }
        else {
         im.setColor(k, QColor(k,k,k).rgb());
            //  puts("139");
        };
       };     
      };
      if (number_bands>=3) {
       im=QImage(ww,hw,QImage::Format_RGB32);
       bpp=4;
       nb_working=3;
      };

      //im.fill(0);
      int wf,hf;// размеры области в локальной СК использумой для отображения

          wf=cf2-cf1+1;
          hf=rf2-rf1+1;

          if ((cf1+wf)>size_raster.width()-1) wf=size_raster.width()-1-cf1;
          if ((rf1+hf)>size_raster.height()-1) hf=size_raster.height()-1-rf1;
          
      // чтение растровых данных
      pdata->RasterIO(GF_Read,cf1,rf1, wf,hf,im.bits(),im.width(),im.height(),GDT_Byte,
      nb_working,list_bands,bpp,im.bytesPerLine(),1);

          // отображение данных
          painter->drawImage(workig_area_in_DSC,im);
     };
        // закрытие файла
     GDALClose(pdata);
    }; 
    };

    // востановление параметров объекта painter
    painter->restore();
  };
}
