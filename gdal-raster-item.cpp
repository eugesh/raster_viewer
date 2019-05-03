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

    m_general_overview = m_raster->get_image(0, 0, m_raster->w(), m_raster->h(), int(m_raster->w() / 8), int(m_raster->h() / 8));

    m_part2draw = m_general_overview;

    return res;
}

QRectF
GDALRasterItem::boundingRect() const {
    /*if(m_part2draw.rect().isValid())
        return QRectF(QPointF(0,0), (m_part2draw.rect().united(m_general_overview.rect())).size());
    else*/
        return QRectF(QPointF(0,0), m_general_overview.size());


    // return QRectF(QPointF(0,0), QSizeF(m_raster->w(), m_raster->h()));
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

    QRectF rect = get_Rect_in_scene_SC();
    // m_image_part2draw = m_raster->get_image(0, 0, m_raster->w(), m_raster->h(), int(scene()->sceneRect().width()), int(scene()->sceneRect().height()));
    m_part2draw = m_raster->get_image(rect.x() * 8, rect.y() * 8, rect.width() * 8, rect.height() * 8, int(rect.width()), int(rect.height()));
    // view();
    painter->drawImage(m_cur_pose, m_general_overview);
    painter->drawRect(0, 0, m_raster->w(), m_raster->h());
    // painter->drawImage(m_cur_pose, m_part2draw);
}

QRectF
GDALRasterItem::get_Rect_in_scene_SC() {
    int width = scene()->views().first()->width();
    int height = scene()->views().first()->height();
    QPointF left_top = scene()->views().first()->mapToScene(0, 0);
    // QPointF left_top = scene()->views().first()->mapToScene(0, 0);

    qDebug() << "viewport in scene SC: x,y= " << left_top << "; w = " << width << "; h = " << height;
    qDebug() << "sceneBoundingRect(): x,y= " << sceneBoundingRect().x() << ", " << sceneBoundingRect().y() << "; w = " << sceneBoundingRect().width() << "; h = " << sceneBoundingRect().height() << ";";
    qDebug() << "QGraphicsItem::pos() = " << QGraphicsItem::pos();
    qDebug() << "pos() = " << pos();
    qDebug() << "visibleRegion() = " << scene()->views().first()->visibleRegion();

    QRectF rect = QRectF(left_top, QSize(width, height)).intersected(sceneBoundingRect());

    qDebug() << "intersect: x,y= " << rect.x() << ", " << rect.y() << "; w = " << rect.width() << "; h = " << rect.height() << ";";

    return rect;
}

void
GDALRasterItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
