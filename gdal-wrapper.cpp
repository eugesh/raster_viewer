#include "gdal-wrapper.hpp"

GDALWrapper::GDALWrapper() {
    zero();
}

GDALWrapper::GDALWrapper(QString const& FileName) {
  _Name=FileName;
  zero() ;
  read(FileName) ;
}

void
GDALWrapper::zero() {
    _pData = nullptr;
    _W = 0;
    _H = 0;
    _NumBands = 0;
    for(int i = 0; i < 3; i++) _BandsList[i] = 0;
    _ProjectionData = QString();
    for(int i = 0; i < 6; i++) _GeoData[i] = 0;

    _nPixelSpace = 0 ;
    // _pColorTable = NULL ;
}

void
GDALWrapper::destroy() {
    if(_pData) {
        GDALClose(_pData) ;
    }

    //if(_pColorTable) delete _pColorTable ;
    zero() ;
}

void
GDALWrapper::setGeoTransform(double *pData) {
    if(! pData)
        return;

    _pData->SetGeoTransform(pData);
    for(int i = 0; i < 6; i++)
        _GeoData[i] = pData[i];
}

bool
GDALWrapper::read(QString FileName) {
    _Name = FileName;
    destroy();

    GDALAllRegister() ;
    //_pData = (GDALDataset*)GDALOpen(FileName.toLocal8Bit().data(), GA_Update ) ;
    _pData = static_cast<GDALDataset*> (GDALOpen(FileName.toLocal8Bit().data(),GA_ReadOnly));
    if(_pData == nullptr) {
        printf("Read : Can't open file <%s>\n",FileName.toLocal8Bit().data());
        return false;
    }

    _W = _pData->GetRasterXSize();
    _H = _pData->GetRasterYSize();

    _ProjectionData.sprintf("%s", _pData->GetProjectionRef());

    _pData->GetGeoTransform(_GeoData);
    _NumBands = _pData->GetRasterCount();
    _BandsList[0] = 1;
    _BandsList[1] = 2;
    _BandsList[2] = 3;

    printf("Read : Read W = %d : H = %d : NumBands = %d\n", _W, _H, _NumBands );
    printf("Read : _ProjectionData = <%s>\n", _ProjectionData.toLocal8Bit().data());

    buildOverviews();

    return true;
}

void
GDALWrapper::buildOverviews() {
    int anOverviewList[3] = { 2, 4, 8 };

    _pData->BuildOverviews( "NEAREST", 3, anOverviewList, 0, nullptr,
                                 GDALDummyProgress, nullptr );

}

QImage
GDALWrapper::compose_rgb_image(float *rbuf, float *gbuf, float *bbuf, int w, int h, float k) {
    QImage rgb_image;

    for(int i=0; i < h; ++i) {
        for(int j=0; j < w; ++j) {
            int red = int(k * rbuf[j + i * w]);
            int green = int(k * gbuf[j + i * w]);
            int blue = int(k * bbuf[j + i * w]);
            rgb_image.setPixel(j, i, qRgb(red, green, blue));
        }
    }

    return rgb_image;
}

/**
 * @brief get_image
 * @param x The pixel offset to the top left corner of the region of the band to be accessed. This would be zero to start from the left side.
 * @param y The line offset to the top left corner of the region of the band to be accessed. This would be zero to start from the top.
 * @param width The width of the region of the band to be accessed in pixels.
 * @param height The height of the region of the band to be accessed in lines.
 * @return QImage.
 */
QImage
GDALWrapper::get_image(int x, int y, int width, int height) {
    GDALRasterBand *pRBand, *pGBand, *pBBand;
    float *rBuffer, *gBuffer, *bBuffer;
    pRBand = _pData->GetRasterBand( 1 );
    pGBand = _pData->GetRasterBand( 2 );
    pBBand = _pData->GetRasterBand( 3 );

    int nXSize = pRBand->GetXSize();
    int nYSize = pRBand->GetYSize();

    // ToDo: read not blocks but scanlines.
    rBuffer = static_cast<float *> (CPLMalloc(sizeof(float) * width * height));
    gBuffer = static_cast<float *> (CPLMalloc(sizeof(float) * width * height));
    bBuffer = static_cast<float *> (CPLMalloc(sizeof(float) * width * height));

    pRBand->RasterIO( GF_Read, x, y, width, height,
                      rBuffer, nXSize, 1, GDT_Float32,
                      0, 0 );

    pGBand->RasterIO( GF_Read, x, y, width, height,
                      gBuffer, nXSize, 1, GDT_Float32,
                      0, 0 );

    pBBand->RasterIO( GF_Read, x, y, width, height,
                      bBuffer, nXSize, 1, GDT_Float32,
                      0, 0 );

    return compose_rgb_image(rBuffer, gBuffer, bBuffer, width, height);
}
