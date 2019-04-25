#ifndef GDALRASTERITEM_H
#define GDALRASTERITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include "gdal-wrapper.hpp"


class GDALRasterItem : public QGraphicsItem
{
public:
    GDALRasterItem(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    //QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=nullptr) override;

    // temporal method.
    void setImage(QImage img) {
        m_image_part2draw = img;
    }

    bool setRaster(QString filePath);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    QImage       m_image;
    QImage       m_image_part2draw;
    QPointF      m_cur_pose;
    GDALWrapper *m_raster;
};

#endif // GDALRASTERITEM_H
