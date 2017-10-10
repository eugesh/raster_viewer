#include <tiff.h>
#include <tiffio.h>

#include "graphic_item_image.h"

//! конструктор
QGraphicsItem_Image::QGraphicsItem_Image(QGraphicsItem * parent ) : QGraphicsItem(parent)
{
  number_bands=0;

  mode_RSC=0;
  //setFlag(QGraphicsItem::ItemIsMovable,true);
}
//! задание имени файла с растровыми данными
void QGraphicsItem_Image::set_image(const QString &name_file,int mode)
{
  prepareGeometryChange ();

  name=name_file;
  size_raster=QSize(0,0);
  number_bands=0;
  mode_RSC=mode;

  if (!name.isEmpty()) {
  	GDALDataset  *pdata;
	GDALDriver *pdriver_tiff;
	bool need_read_tiff=false;

	pdriver_tiff=(GDALDriver *)GDALGetDriverByName ("GTiff");
	
  	pdata=(GDALDataset*)GDALOpen(name.toLocal8Bit().data(),GA_ReadOnly);
  	if (pdata!=0) {
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
		if (number_bands>0) {
				GDALRasterBand *pband;

				pband=pdata->GetRasterBand(1);

				band_min=0;band_max=255;
				switch(pband->GetRasterDataType()) {
					case GDT_UInt16: ;
						band_min=0;band_max=65535;
						need_read_tiff=true;
						break;
					case GDT_Int16: ;
						band_min=-32768 ;band_max=32767;
						need_read_tiff=true;
						break;				
				};
/*
				int k;
				for(k=1;k<=number_bands;k++) {
					double bmin,bmax;

					pband=pdata->GetRasterBand(k);
					pband->GetStatistics(TRUE,TRUE,&bmin,&bmax,0,0);
					printf("smin=%lf smax=%lf\n",bmin,bmax);
					if (k==1) {
						band_min=bmin;
						band_max=bmax;
					}
					else {
						if (bmin<band_min) band_min=bmin;
						if (bmax>band_max) band_max=bmax;
					};
				};
				printf("band_min=%d band_min=%d\n",band_min,band_max);
				*/
				
		};	  
  		// закрытие файла
  		GDALClose(pdata);
		if (need_read_tiff) {
			TIFF* ptiff;
			quint16 smin,smax,count;

			ptiff=TIFFOpen(name.toLocal8Bit().data(),"r");

			if (ptiff!=0) {
				TIFFGetFieldDefaulted(ptiff,TIFFTAG_MINSAMPLEVALUE,&smin);
				TIFFGetFieldDefaulted(ptiff,TIFFTAG_MAXSAMPLEVALUE,&smax);

				band_min=smin;
				band_max=smax;

				TIFFClose(ptiff);
			};

			printf("band_min=%d band_max=%d\n",band_min,band_max);
		};		
  	};    
  };
}

//! получение границ
QRectF QGraphicsItem_Image::boundingRect () const
{
  //return QRectF(0,0,size_raster.width()+1,size_raster.height()+1) ;
  return QRectF(0,0,size_raster.width(),size_raster.height()) ;
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
    
   // printf("1: cf1=%lf cf2=%lf rf1=%lf rf2=%lf\n",cf1,cf2,rf1,rf2);
    // определение области растровых данных требуемой для отображения
    workig_area_in_LSC=bound.intersected(QRectF(window_rect_in_LSC));
    
    workig_area_in_LSC.getCoords(&cf1, &rf1, &cf2, &rf2);
    cf1=floor(cf1);
    rf1=floor(rf1);
    cf2=ceil(cf2);
    rf2=ceil(rf2);

    workig_area_in_LSC.setCoords(cf1, rf1, cf2, rf2);
    
    
    // вычисление области отображаемых данных в СК устройства отображения
    workig_area_in_DSC=transform.mapRect(workig_area_in_LSC);

    // получение граничных точек прямоугольной области 
    
    workig_area_in_DSC.getCoords(&xw1, &yw1, &xw2, &yw2);

    //printf("W=%d H=%d\n",W,H);
    //printf("m11=%lf m21=%lf m12=%lf m22=%lf dx=%lf dy=%lf\n",
    //  transform.m11(),transform.m21(),transform.m12(),transform.m22(),
   //   transform.dx(),transform.dy());

    //printf("2: cf1=%lf cf2=%lf rf1=%lf rf2=%lf\n",cf1,cf2,rf1,rf2);

   // printf("xw1=%lf xw2=%lf yw1=%lf yw2=%lf\n",xw1,xw2,yw1,yw2);
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
          ww=xw2-xw1;
          hw=yw2-yw1;

          //printf("ww=%d hw=%d\n",ww,hw);

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
  						}
  						else {
  							im.setColor(k, QColor(k,k,k).rgb());
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

		
		if (mode_RSC==1) {
			int c1p,c2p,r1p,r2p;

			c1p=size_raster.width()-cf2;
			c2p=size_raster.width()-cf1;

			r1p=size_raster.height()-rf2;
			r2p=size_raster.height()-rf1;

			cf1=c1p;
			cf2=c2p;

			rf1=r1p;
			rf2=r2p;
		};

		// чтение растровых данных
          	wf=cf2-cf1;
         	hf=rf2-rf1;

          //if ((cf1+wf)>size_raster.width()-1) wf=size_raster.width()-1-cf1;
          //if ((rf1+hf)>size_raster.height()-1) hf=size_raster.height()-1-rf1;

          //printf("wf=%d hf=%d\n",wf,hf);
          //printf("mode=%d  cf1=%lf cf2=%lf rf1=%lf rf2=%lf\n",mode_RSC,cf1,cf2,rf1,rf2);
          
  				/*
  				pdata->RasterIO(GF_Read,cf1,rf1, wf,hf,im.bits(),im.width(),im.height(),GDT_Byte,
  				nb_working,list_bands,bpp,im.bytesPerLine(),1);
  				*/
          		{
          			QByteArray raster_data(im.width()*im.height()*nb_working*4,0);
					
          			pdata->RasterIO(GF_Read,cf1,rf1, wf,hf,raster_data.data(),im.width(),im.height(),GDT_Int32,
  				nb_working,list_bands,nb_working*4,im.width()*nb_working*4,4);

				int i,j;
				double R,G,B,C_max;
				int pos_pixel;
				quint32 color;

				C_max=255.0/(double(band_max)-band_min);
				const qint32 *pdata;

				pdata=(const qint32 *)raster_data.data();
				

				for(i=0;i<im.height();i++) {
					for(j=0;j<im.width();j++) {
						pos_pixel=i*im.width()*nb_working+j*nb_working;

						if (nb_working==3) {
							R=(pdata[pos_pixel+2]-band_min)*C_max;
							G=(pdata[pos_pixel+1]-band_min)*C_max;
							B=(pdata[pos_pixel+0]-band_min)*C_max;
							color=qRgb(R,G,B);
						};
						if (nb_working==1) {
							color=(pdata[pos_pixel+0]-band_min)*C_max;
						};

						im.setPixel(j,i,color);
					};
				};				
          		};
		  
		  if (mode_RSC==1) {
		  	//трансформирование считанного фрагмента растровых данных
		  	int i,j;
			quint8 *pdata,*pdata2;
			QImage im2;

			im2=im;

			pdata=im.bits();
			pdata2=im2.bits();

			for(i=0;i<im.height();i++) {
				for(j=0;j<im.width();j++) {
					void *pos1,*pos2;
					pos1=pdata+i*im.width()*bpp+j*bpp;
					pos2=pdata2+(im.height()-1-i)*im.width()*bpp+(im.width()-1-j)*bpp;
					memcpy(pos1,pos2,bpp);
				};
			};
		  };

          // отображение данных
          painter->drawImage(xw1,yw1,im);
  			};
        // закрытие файла
  			GDALClose(pdata);
  		}; 
    };

    // востановление параметров объекта painter
    painter->restore();
  };
}
