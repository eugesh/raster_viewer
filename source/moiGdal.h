#ifndef __CGDALRASTER_H__
#define __CGDALRASTER_H__

#include <QImage>
#include <QString>
#include <QMatrix>
#include <QColor>
#include <QFile>
#include <gdal_priv.h>
#include <gdal.h> 

class CGDALRaster {
public:
  //!< Pointer to data
  GDALDataset*_pData ;
private: ;
  int _W, _H ; //!< Width, Height raster
  int _NumBands ; //!< Number of bands in raster
  int _BandsList[3] ; //!< List of bands in raster
  double _GeoData[6] ; //!< Affine transformation
  QString _ProjectionData ; //!< Projection Name
  int _nPixelSpace ; //!< number of pixels between adjacent pixels
  QString Name;
  // GDALColorTable*_pColorTable ; //!< The Color Table

private: ;

  void Zero() {
    _pData = NULL ;
    _W = 0, _H = 0 ;
    _NumBands = 0 ;
    for( int i = 0 ; i < 3 ; i ++ ) _BandsList[i] = 0 ;
    _ProjectionData = QString() ;
    for( int i = 0 ; i < 6 ; i ++ ) _GeoData[i] = 0 ;

    _nPixelSpace = 0 ;
    // _pColorTable = NULL ;
  } ;

  void Destroy() {
    if(_pData) {
      GDALClose(_pData) ;
    } ;

    //if(_pColorTable) delete _pColorTable ;
    Zero() ;
  } ;
public: ;

bool isNull()const { return !_pData ? true : false ; } ;

int Width()const { return _W ; };

int Height()const { return _H ; };

QString GetProjectionData()const { return _ProjectionData ; } ;

void SetGeoTransform(double *pData) {
_pData->SetGeoTransform(pData) ;
for( int i = 0 ; i < 6 ; i ++ )
  _GeoData[i] = pData[i] ;
} ;

void SetProjectionData(QString const&ProjectionData) {
_pData->SetProjection( ProjectionData.toLocal8Bit().data() ) ;
_ProjectionData = ProjectionData ;
} ;

CGDALRaster() { 
  Zero() ;
} ;

CGDALRaster(QString const&FileName) { 
  Name=FileName;
  Zero() ; 
  Read(FileName) ;
} ;

~CGDALRaster() { 
  Destroy() ;
} ;

void set_path(QString const&FileName){ 
  Name=FileName;  
  Zero() ; 
  Read(FileName) ;
} 

bool Read(const QString &FileName){
    Name=FileName;
    Destroy() ;

    GDALAllRegister() ;
    //_pData = (GDALDataset*)GDALOpen(FileName.toLocal8Bit().data(), GA_Update ) ;
    _pData = (GDALDataset*)GDALOpen(FileName.toLocal8Bit().data(),GA_ReadOnly);
    if( _pData==0 ) {
       printf("Read : Can't open file <%s>\n",FileName.toLocal8Bit().data());
       return false ;
    } ;

    _W = _pData->GetRasterXSize() ;
    _H = _pData->GetRasterYSize() ;

    _ProjectionData.sprintf("%s",_pData->GetProjectionRef()) ;    

    _pData->GetGeoTransform(_GeoData) ;
    _NumBands = _pData->GetRasterCount() ;
    _BandsList[0] = 1 ; 
    _BandsList[1] = 2 ; 
    _BandsList[2] = 3 ;

    printf("Read : Read W = %d : H = %d : NumBands = %d\n", _W, _H, _NumBands ) ;
    printf("Read : _ProjectionData = <%s>\n", _ProjectionData.toLocal8Bit().data()) ;

    return true ;
} ;

bool CreateGeoTIFF( QString const&filename, int W, int H, 
                      /*double*pGeoData,*/ QString const&PrjName ) {
    Name=filename;                      
    Destroy() ;
    GDALDriver *pdriver;

    GDALAllRegister() ;
    GDALDriver*pDriver = (GDALDriver*) GDALGetDriverByName("GTiff" ) ;

    // Создание доп. файла
    // char **papszOptions = NULL;
    // papszOptions = CSLSetNameValue( papszOptions, "TFW", "YES" ) ;
    char block_size_str[100];
    char **pcreate_str = 0;
    _W = W ;
    _H = H ;

    // Cоздаем цветной GeoTiff
    _NumBands = 3 ;
    _BandsList[0] = 1 ; 
    _BandsList[1] = 2 ; 
    _BandsList[2] = 3 ;
    _nPixelSpace = 4 ;

    // int*pBandList = _BandsList ;
    // int nBandSpace = 1 ;

    QString filenameTiff = filename + ".tif" ;

    sprintf(block_size_str, "%d", 128);

    // Задание размера блока
    pcreate_str = CSLSetNameValue(pcreate_str, "BLOCKXSIZE", block_size_str);
    pcreate_str = CSLSetNameValue(pcreate_str, "BLOCKYSIZE", block_size_str);

    // pcreate_str=CSLSetNameValue(pcreate_str,"BIGTIFF","YES");
    pcreate_str=CSLSetNameValue(pcreate_str,"TILED","YES");
    _pData = pDriver->Create(filenameTiff.toLocal8Bit().data(), W, H, _NumBands, GDT_Byte, pcreate_str);
    if(!_pData){
      printf("CreateGeoTIFF : Can't create file <%s>\n", filenameTiff.toLocal8Bit().data());
      return false;
    } ;

    // if(pGeoData) SetGeoTransform(pGeoData) ;

    if(!PrjName.isNull()) SetProjectionData(PrjName) ;

    return true ;
} ;

bool CreateGeoTIFF_gray( QString const&filename, int W, int H, 
                      /*double*pGeoData,*/ QString const&PrjName ) {
    Name=filename;                      
    Destroy() ;
    GDALDriver *pdriver;

    GDALAllRegister() ;
    GDALDriver*pDriver =(GDALDriver*) GDALGetDriverByName("GTiff" ) ;

    // Создание доп. файла
    // char **papszOptions = NULL;
    // papszOptions = CSLSetNameValue(papszOptions, "TFW", "YES") ;
    char block_size_str[100];
    char **pcreate_str=0;
    _W = W ;
    _H = H ;

    // Cоздаем цветной GeoTiff
    _NumBands = 1 ;
    _BandsList[0] = 1 ; 
    _BandsList[1] = 2 ; 
    _BandsList[2] = 3 ;
    _nPixelSpace = 0 ;

    // int*pBandList = _BandsList ;
    // int nBandSpace = 1 ;

    QString filenameTiff = filename + ".tif" ;

    sprintf(block_size_str, "%d", 128);
    // задание размера блока
    pcreate_str = CSLSetNameValue(pcreate_str, "BLOCKXSIZE", block_size_str);
    pcreate_str = CSLSetNameValue(pcreate_str, "BLOCKYSIZE", block_size_str);

    //pcreate_str=CSLSetNameValue(pcreate_str,"BIGTIFF","YES");
    pcreate_str=CSLSetNameValue(pcreate_str, "TILED", "YES");

    _pData = pDriver->Create( filenameTiff.toLocal8Bit().data(), W, H, _NumBands, GDT_Byte, pcreate_str ) ;
    if(!_pData){
      printf("CreateGeoTIFF : Can't open file <%s>\n", filenameTiff.toLocal8Bit().data()) ;
      return false ;
    } ;

    //if(pGeoData) SetGeoTransform(pGeoData) ;
    if(!PrjName.isNull()) SetProjectionData(PrjName) ;

    return true ;
} ;

QImage GetQImageRGB32( QPoint const&Pos, int W, int H ) {
    return GetQImageRGB32(Pos, W, H, W, H);
}

QImage GetQImageRGB32(QPoint const&Pos, int BaseW, int BaseH, int NewW, int NewH) {
    QImage Img(NewW, NewH, QImage::Format_RGB32);
    Img.fill(0) ; // обнуление изображения
    _nPixelSpace = 4;
    // чтение растровых данных
    _pData->RasterIO(GF_Read, Pos.x(), Pos.y(), BaseW, BaseH,
                     Img.bits(), Img.width(), Img.height(), GDT_Byte,
                     _NumBands, _BandsList, _nPixelSpace, Img.bytesPerLine(), 1);

    return Img;
} ;

QImage GetQImageGray(QPoint const&Pos, int W, int H) {
    return GetQImageGray(Pos, W, H, W, H);
}

QImage GetQImageGray( QPoint const&Pos, int BaseW, int BaseH, int NewW, int NewH ) {
    QImage Img(NewW, NewH, QImage::Format_Indexed8);
    Img.fill(0) ; // обнуление изображения
    Img.setNumColors(256);
    for( int i = 0 ; i < 256 ; i ++ ) {
        Img.setColor( i, QColor(i,i,i).rgb( ) ) ;     
    } ;
    _nPixelSpace = 0;
    int numBands =1;
    // чтение растровых данных
    _pData->RasterIO( GF_Read, Pos.x(), Pos.y(), BaseW, BaseH,
                      Img.bits(), Img.width(),Img.height(), GDT_Byte,
                      numBands, _BandsList, _nPixelSpace, Img.bytesPerLine(), 1 );
    return Img ;
} ;

bool WriteRGB32( QPoint const&Pos, QImage const&Img ) { return WriteRGB32(Pos.x(), Pos.y(), Img) ; } ;
  
bool WriteRGB32( int Xoffset, int Yoffset, QImage const&Img ) {
    QImage ImgRGB(Img.width(),Img.height(),QImage::Format_RGB32);
    int color;

    for(int i = 0; i < Img.height(); ++i)
       for(int j = 0; j < Img.width(); ++j) {
          ImgRGB.setPixel(j, i, Img.pixel(j, i));
       }       

    if(!_pData) return false ;

    int W = Img.width() ;
    int H = Img.height() ;

    int NumBands = _NumBands ;
    int*pBandList = _BandsList ;
    int nPixelSpace = _nPixelSpace ;

    NumBands = 3;
    nPixelSpace = 4;
    pBandList = NULL;
    
    int nBandSpace = 1 ;

    CPLErr ErrStatus=_pData->RasterIO(GF_Write,Xoffset,Yoffset,W,H,(void*)ImgRGB.bits(),W,H,GDT_Byte,
                                      NumBands,pBandList,nPixelSpace,ImgRGB.bytesPerLine(),nBandSpace);
    bool isSuccess =  true ;
    if((int)ErrStatus != 0) {
      printf("Write : ErrStatus = %d\n", (int)ErrStatus) ;
      isSuccess = false ;
    } ;

    return isSuccess ;
} ;
bool WriteGray( QPoint const&Pos, QImage const&Img ) { return WriteGray(Pos.x(), Pos.y(), Img) ; } ;
  
bool WriteGray( int Xoffset, int Yoffset, QImage const&Img ) {
    QImage ImgRGB(Img.width(),Img.height(),QImage::Format_Indexed8);
    int color;
    ImgRGB.setNumColors(256) ;

    for( int i = 0 ; i < 256 ; i ++ ) {
          ImgRGB.setColor( i, QColor(i,i,i).rgb( ) ) ;  
    } ;
    for(int i = 0; i < Img.height();++i)
       for(int j = 0; j < Img.width();++j) {
          color=qGray(Img.pixel(j,i));
          ImgRGB.setPixel(j,i,color);     
       }       

    if(!_pData) return false ;

    int W = Img.width() ;
    int H = Img.height() ;

    int NumBands = _NumBands ;
    int*pBandList = _BandsList ;
    int nPixelSpace = _nPixelSpace ;

    NumBands = 1;
    nPixelSpace = 0;
    pBandList = NULL;
    
    int nBandSpace = 1 ;

    CPLErr ErrStatus=_pData->RasterIO(GF_Write,Xoffset,Yoffset,W,H,(void*)ImgRGB.bits(),W,H,GDT_Byte,
                                      NumBands,pBandList,nPixelSpace,ImgRGB.bytesPerLine(),nBandSpace);
    bool isSuccess =  true ;
    if((int)ErrStatus != 0) {
      printf("Write : ErrStatus = %d\n", (int)ErrStatus) ;
      isSuccess = false ;
    } ;

    return isSuccess ;
} ;

void create_overviews() {
     int numCount = (_pData->GetRasterBand(_pData->GetRasterCount()))->GetOverviewCount(); 
     QFile ovrfile(Name+".ovr");
        if(!ovrfile.exists()&&numCount==0) {
            int anOverviewList[3] = { 4, 8, 16 };
            _pData->BuildOverviews("NEAREST",3,anOverviewList,0,NULL,NULL,NULL);
        }  
 }

};
#endif
