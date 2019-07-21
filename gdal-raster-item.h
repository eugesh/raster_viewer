#ifndef GDALRASTERITEM_H
#define GDALRASTERITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsView>
#include "gdal-wrapper.hpp"


class QGraphicsScene;
class QGraphicsView;

static const int ovConst = 8;

class GDALRasterItem : public QGraphicsItem
{
public:
    GDALRasterItem(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    //QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=nullptr) override;

    // temporal method.
    void setImage(QImage img) {
        m_general_overview = img;
    }

    bool setRaster(QString filePath);
    QSize getRasterSize() { return m_raster_size; }
    void change_scale(double scale);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QRectF get_Rect_in_scene_SC();

protected:
    QImage       m_image;
    QImage       m_general_overview;
    QImage       m_part2draw;
    QPointF      m_cur_cursor_pose;
    GDALWrapper *m_raster;
    QSize        m_raster_size;
    QPointF      m_real_move; // Shift in pixels of raster.
    double       m_scale;
    QPointF      m_prev_pose;
    QPointF      m_start_pose;
    QRectF       m_win_in_raster; // Scaled and intersected with scene it is a part of raster to show.
    QMatrix      m_ov_matrix;
    QMatrix      m_raster_matrix;
    QMatrix      m_viewport_on_scene_matrix;
};

#endif // GDALRASTERITEM_H
