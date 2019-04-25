#ifndef GDALWRAPPER_HPP
#define GDALWRAPPER_HPP

#include <QImage>
#include <QString>
#include <QMatrix>
#include <QColor>
#include <QFile>
#include <gdal_priv.h>
#include <gdal.h>


class GDALWrapper {
public:
    GDALWrapper();
    GDALWrapper(QString const&FileName);

    //!< Pointer to data
    GDALDataset * _pData ;

public:
    bool isNull() const { return !_pData ? true : false; }
    int w() const { return _W; }
    int h() const { return _H; }
    QString getProjectionData() const { return _ProjectionData; }

    void setGeoTransform(double *pData);
    bool read(QString FileName);
    QImage get_image(int x, int y, int width, int height);
    static QImage compose_rgb_image(float *rbuf, float *gbuf, float *bbuf, int w, int h, float k=1);

protected:
    void zero();
    void destroy();
    void buildOverviews();

protected:
    int _W, _H; //!< Width, Height raster
    int _NumBands; //!< Number of bands in raster
    int _BandsList[3]; //!< List of bands in raster.
    double _GeoData[6]; //!< Affine transformation.
    QString _ProjectionData; //!< Projection name.
    int _nPixelSpace; //!< Number of pixels between adjacent pixels.
    QString _Name;
    // GDALColorTable*_pColorTable ; //!< The Color Table
};

#endif // GDALWRAPPER_HPP
