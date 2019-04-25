#include <QRectF>
#include <qdebug.h>
#include <QGraphicsSceneMouseEvent>
// #include "image-item.h"
#include "gdal-raster-item.h"


GDALRasterItem::GDALRasterItem(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    setZValue(100);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

    m_raster_size = QSize(0,0);
}

bool
GDALRasterItem::setRaster(QString filePath) {
    bool res = true;

    m_raster = new GDALWrapper(filePath);

    m_raster_size = QSize(m_raster->w(), m_raster->h());

    m_image_part2draw = m_raster->get_image(0, 0, m_raster->w(), m_raster->h(), int(m_raster->w() / 8), int(m_raster->h() / 8));

    return res;
}

QRectF
GDALRasterItem::boundingRect() const {
    return QRectF(QPointF(0,0), m_image_part2draw.size());
}

/*QPainterPath
GDALRasterItem::shape() const
{
    QPainterPath path;
    path.addRect(14, 14, 82, 42);
    return path;
}*/

void
GDALRasterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(option);
    // painter->drawImage(QPoint(0,0), m_image_part2draw);
    qDebug() << "w = " << int(scene()->sceneRect().width());
    qDebug() << "h = " << int(scene()->sceneRect().height());
    // m_image_part2draw = m_raster->get_image(0, 0, m_raster->w(), m_raster->h(), int(scene()->sceneRect().width()), int(scene()->sceneRect().height()));
    painter->drawImage(m_cur_pose, m_image_part2draw);
}

void GDALRasterItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void GDALRasterItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        m_cur_pose = event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void GDALRasterItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
