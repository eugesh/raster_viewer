#include <stdio.h>

#include <math.h>

#include <QApplication>
#include <QTextCodec>
#include <QTime>
#include <QUdpSocket>
#include <QHostAddress>
#include <QProcess>
#include <QImage>
#include <QFile>
#include <QTextStream>
#include <QGraphicsView>
#include <gdal_priv.h> 

//#include "form_mainwindow_krest.h"
#include "Krest_window.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    //QApplication::addLibraryPath(QObject::tr("./plugins"));
 
    GDALAllRegister();
 //   Ui::MainWindow ui;
//    KrestWindow view;
    KrestWindow view;
//    ui.setupUi(view);
    view.show();
    return app.exec();
}

/*int main(int argc, char **argv)
{
   CGDALRaster raster;
   CGDALRaster rasterOut;
   CGDALRaster rasterOut_gray;
   double GeoData[6] ;
   QString path="E:/Shtanov/images/img_doc_1_center.bmp";
   raster.Read(path);
   rasterOut.CreateGeoTIFF("E:/Shtanov/projects/interface_krest_new2/transform/test_RASTER",164,164,GeoData,raster.GetProjectionData());
   rasterOut_gray.CreateGeoTIFF("E:/Shtanov/projects/interface_krest_new2/transform/test_RASTER_gray",164,164,GeoData,raster.GetProjectionData());
   QImage img;
   QImage img_gray;   
   QPoint point(0,0);
   img=raster.GetQImageRGB32(point, 164, 164);
   img_gray=raster.GetQImageGray(point, 164, 164);
   img.save("E:/Shtanov/projects/interface_krest_new2/transform/test.bmp");
   img_gray.save("E:/Shtanov/projects/interface_krest_new2/transform/test_gray.bmp");
   rasterOut.WriteRGB32(0,0,img);
   rasterOut_gray.WriteGray(0,0,img_gray);
   return 0;
}*/
/*int main(int narg,char **varg)
{
  QApplication app(narg,varg);

  QTextCodec::setCodecForTr(QTextCodec::codecForName ("Windows-1251"));

  QApplication::addLibraryPath(QObject::tr("./plugins"));
 
  GDALAllRegister(); 

  QGraphicsItem_Image *pitem;
  	
  QGraphicsScene scene;

  pitem=new QGraphicsItem_Image;
  scene.addItem(pitem);

  //pitem->set_image("D:/test_r/00060.tif");
  pitem->set_image(QObject::tr("E:/Shtanov/projects/interface_krest/images/img.bmp"));


  QGraphicsView view;

  view.setScene(&scene);
  view.scale(2, 2);

  view.show();

  app.exec();
  

  printf("end\n");
  getchar();
  return 0;
}
*/
