#include "transformation.h"
#include "moiGdal.h"
#include <QVector3D>
#include <iostream>

settings::settings( ) {
   q_out_size = QSize(0, 0);
}

double roundD(double x) {
  return ((x - floor(x)) >= 0.5) ? ceil(x) : floor(x);
}

int transformation::run_process(abstractProgress*) {
  int W_IN = rasterIn.Width ();
  int H_IN = rasterIn.Height();
  int W_OUT;
  int H_OUT;
  if(q_out_size.width() <= 0 || q_out_size.height() <= 0) {
     W_OUT = qMax(W_IN, H_IN);
     H_OUT = qMax(W_IN, H_IN);
  }
  else {
      W_OUT= q_out_size.width();
      H_OUT= q_out_size.height();
  }

  double GeoData[6] ;

  QVector3D ro[4], roIN[4];
  QVector3D e1, e2, b, rc;
  QVector3D la1, la2, la;
  double r0,c0;
  QPointF XL, XR, YU, YD;
  XL = qvqp_x[0];
  YU = qvqp_x[1];
  XR = qvqp_x[2];
  YD = qvqp_x[3];
  ro[0]=QVector3D(YD.x(), YD.y(), 0);   // координаты крестов
  ro[1]=QVector3D(YU.x(), YU.y(), 0);   //   1
  ro[2]=QVector3D(XL.x(), XL.y(), 0);   // 2   3
  ro[3]=QVector3D(XR.x(), XR.y(), 0);   //   0
  e1=ro[3] - ro[2];  // направленийа осей координат √ео— 
  e2=ro[1] - ro[0];  // во входном снимке

  // Нормированный базис √ео—  входного снимка.
  e1=e1.normalized ();
  e2=e2.normalized ();

  b = ro[0] - ro[2];

  double e11, e12, e22, e1b, e2b;
  double D, D1, t1;

  // Cкалярные произведения.
  e11 = QVector3D::dotProduct(e1,e1);
  e12 = QVector3D::dotProduct(e1,e2);
  e22 = QVector3D::dotProduct(e2,e2);
  e1b = QVector3D::dotProduct(e1,b);
  e2b = QVector3D::dotProduct(e2,b);

  // Преобразование.
  D =- e11 * e22 + e12 * e12;
  D1 =- e1b * e22 + e2b * e12;

  t1 = D1 / D;
  
  rc=ro[2] + e1 * t1;
  // Создание выходного растра в формате .TIF.
  if(!rasterOut.CreateGeoTIFF(name_out, W_OUT, H_OUT,
                                GeoData, rasterIn.GetProjectionData())) {
      puts("rasterOut.CreateGeoTIFF(name_out, truble");
      return -1;
  }
  
  // Размеры блока по умолчанию.
  int BlockSizeX_OUT = MAX_SIZE_BLOCK;
  int BlockSizeY_OUT = MAX_SIZE_BLOCK;
  // Кол-во блоков.
  int NumBlocksX_OUT = (int) ceil(double(W_OUT) / double(BlockSizeX_OUT));
  int NumBlocksY_OUT = (int) ceil(double(H_OUT) / double(BlockSizeY_OUT));
  // Рассчет размеров блока.
  if ( BlockSizeX_OUT >= W_OUT)
    BlockSizeX_OUT= W_OUT;
  else
    BlockSizeX_OUT= (int)((double)W_OUT / (double) NumBlocksX_OUT);
  if ( BlockSizeY_OUT >= H_OUT)
    BlockSizeY_OUT= H_OUT;
  else
    BlockSizeY_OUT= (int)((double)H_OUT / (double) NumBlocksY_OUT);

  // Координаты центра выходного снимка.
  r0=H_OUT / 2.0;
  c0=W_OUT / 2.0;

  double cp, rp;
  double dc, dr;
  int icp, irp;
  QVector3D point;  

  QImage ijBlock_OUT(BlockSizeX_OUT, BlockSizeY_OUT, QImage::Format_RGB32);
  ijBlock_OUT.fill(0);

  QPoint ijPosB;
  // Цикл по всем блокам для поворота растра.
  progressbar->setMinimum(0);
  progressbar->setMaximum(NumBlocksY_OUT * NumBlocksX_OUT - 1);
  for( int iBlock = 0 ; iBlock < NumBlocksY_OUT; iBlock++ ) {
    for( int jBlock = 0 ; jBlock < NumBlocksX_OUT; jBlock++ ) {
       // Очередные координаты блока в выходном растре.
       QPoint ijPos(jBlock * BlockSizeX_OUT, iBlock * BlockSizeY_OUT) ;
       // Читаем блок результирующей карты.
       QPoint pb[4];
       pb[0] = ijPos;
       pb[1] = ijPos + QPoint(BlockSizeX_OUT, 0);               //12
       pb[2] = ijPos + QPoint(BlockSizeX_OUT, BlockSizeY_OUT);  //43
       pb[3] = ijPos + QPoint(0, BlockSizeY_OUT); //координаты углов блока в выходном растре
    
       QPointF pbIN[4];
       double dcIN[4], drIN[4];
       for(int k = 0; k < 4; ++k) {
           dcIN[k] = pb[k].x() - c0;
           drIN[k] = r0 - pb[k].y();
           point = rc + e1 * dcIN[k] + e2 * drIN[k];
           pbIN[k] = point.toPoint();     //координаты углов блока во входном растре
       }
       for(int k = 0; k < 4 ; ++k) {
          if(pbIN[k].x() < 0)pbIN[k].setX(0);
          if(pbIN[k].y() < 0)pbIN[k].setY(0);
          if(pbIN[k].x() > W_IN)pbIN[k].setX(W_IN);
          if(pbIN[k].y() > H_IN)pbIN[k].setY(H_IN);
       }
       double maxX, maxY, minX, minY;
       maxX=pbIN[0].x();
       maxY=pbIN[0].y();    
       minX=pbIN[0].x();    
       minY=pbIN[0].y();    
       for(int k = 0; k < 4; ++k) {
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
       QImage ijBlock_IN(BlockSizeX_IN,BlockSizeY_IN, QImage::Format_Indexed8);//блок входного растра, чб, тк растр чб
       QPoint ijPosIN(floor(minX),floor(minY)); 
       fprintf(stdout,"block: iBlock = %d : jBlock = %d\n", iBlock, jBlock);
       fprintf(stdout,"block: BlockSizeX_IN = %d : BlockSizeY_IN = %d\n", BlockSizeX_IN, BlockSizeY_IN);
       // читаем блок исходного растра
       ijBlock_IN= rasterIn.GetQImageGray(ijPosIN, BlockSizeX_IN, BlockSizeY_IN);        
       for(int r = 0; r < BlockSizeY_OUT; r++) {
          for(int c = 0; c < BlockSizeX_OUT; c++) {
             // Координаты точек блока внутри растра = координаты блока + координаты внутри блока
             ijPosB = ijPos + QPoint(c, r); //в выходном растре

             dc = ijPosB.x() - c0;   //относительно центра
             dr = r0 - ijPosB.y();
             point = rc + e1 * dc + e2 * dr; //координаты во входном снимке
             cp=point.x();  //координаты во входном снимке
             rp=point.y();
             icp = (int)roundD(cp); //координаты во входном снимке в типе инт
             irp = (int)roundD(rp);

             int color;
             QRgb qrgb_color;
             if( icp < W_IN && icp >= 0 ) {
                 if( irp < H_IN && irp >= 0 ) {
                     if(icp-ijPosIN.x( ) < BlockSizeX_IN && irp - ijPosIN.y() < BlockSizeY_IN)
                         color = qGray(ijBlock_IN.pixel(icp - ijPosIN.x(), irp - ijPosIN.y()));
                     else
                         color = qGray(0, 0, 0);
                 }
                 else
                     color = qGray(0, 0, 0);
             }
             else
                 color = qGray(0, 0, 0);

             ijBlock_OUT.setPixel(c, r, qRgb(color, color, color));
         }
       }
       rasterOut.WriteRGB32( ijPos, ijBlock_OUT );
       // Emit signal with number of processed blocks
       emit number_worked_file(iBlock * NumBlocksX_OUT + jBlock);
    }
  }
  rasterOut.create_overviews( );
  return 0;
}

