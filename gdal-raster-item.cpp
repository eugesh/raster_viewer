#include <QRectF>
#include <qdebug.h>
#include <QGraphicsSceneMouseEvent>
#include <QScrollBar>

// #include "image-item.h"
#include "gdal-raster-item.h"
// #include "affine_tr.hpp"


static const double EPS = 0.000001;


GDALRasterItem::GDALRasterItem(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    setZValue(100);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

    m_raster_size = QSize(0,0);
    m_real_move = QPointF(0,0);
}

bool
GDALRasterItem::setRaster(QString filePath) {
    bool res = true;

    m_raster = new GDALWrapper(filePath);

    m_raster_size = QSize(m_raster->w(), m_raster->h());

    m_general_overview = m_raster->get_image(0, 0, m_raster->w(), m_raster->h(), int(m_raster->w() / ovConst), int(m_raster->h() / ovConst));

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
    painter->save();
    // painter->drawImage(QPoint(0,0), m_image_part2draw);
    qDebug() << "w = " << int(scene()->sceneRect().width());
    qDebug() << "h = " << int(scene()->sceneRect().height());

    QRectF rect = get_Rect_in_scene_SC();
    // m_image_part2draw = m_raster->get_image(0, 0, m_raster->w(), m_raster->h(), int(scene()->sceneRect().width()), int(scene()->sceneRect().height()));
    // m_part2draw = m_raster->get_image(rect.x() * ovConst, rect.y() * ovConst, rect.width() * ovConst, rect.height() * ovConst, int(rect.width()), int(rect.height()));
    // view();
    painter->drawImage(QPoint(0, 0), m_general_overview);
    // painter->drawRect(0, 0, m_raster->w(), m_raster->h());
    // painter->drawImage(m_cur_pose, m_part2draw);

    // Debug: draw viewport rect on shot.
    QRectF viewport_rect = scene()->views().first()->viewport()->rect();
    viewport_rect.setWidth(viewport_rect.width() / m_scale);
    viewport_rect.setHeight(viewport_rect.height() / m_scale);
    viewport_rect.moveTo(m_real_move.rx() / ovConst - viewport_rect.width() / 2, m_real_move.ry() / ovConst - viewport_rect.height() / 2);

    QRectF viewport = scene()->views().first()->viewport()->rect();

    // painter->set

    painter->drawRect(viewport_rect);
    painter->setPen(Qt::red);
    QRectF sceneRect = sceneBoundingRect();
    // scenRect.moveTo(m_real_move.rx() / ovConst, m_real_move.ry() / ovConst);
    QRectF inter_rect = viewport_rect.intersected(sceneRect);
    painter->drawRect(inter_rect);
    painter->restore();

    // Get fragment to draw.
    painter->save();
    painter->setMatrix(m_ov_matrix);
    // double raster_x = inter_rect.x() * ovConst / m_scale;
    // double raster_y = inter_rect.y() * ovConst;
    double raster_x = m_viewport_on_scene_matrix.dx() * ovConst;
    double raster_y = m_viewport_on_scene_matrix.dy() * ovConst;
    m_part2draw = m_raster->get_image(raster_x, raster_y, inter_rect.width() * ovConst * m_scale, inter_rect.height() * ovConst * m_scale, int(inter_rect.width()), int(inter_rect.height()));
    // painter->drawImage(QPoint(inter_rect.x(), inter_rect.x()), m_part2draw);
    painter->drawImage(QPoint(0, 0), m_part2draw);

    // painter->drawImage(QPoint(m_ov_matrix.dx(), m_ov_matrix.dy()), m_part2draw);
    painter->restore();
}

QRectF
GDALRasterItem::get_Rect_in_scene_SC() {
    QRect rect = scene()->views().first()->viewport()->rect();

    qDebug() << "viewport() = " << rect;

    QPointF leftTop = scene()->views().first()->mapToScene(rect.topLeft());
    QPointF rightBot = scene()->views().first()->mapToScene(rect.bottomRight());

    qDebug() << "viewport() = " << leftTop << rightBot;

    qDebug() << "m_cur_cursor_pose = " << m_cur_cursor_pose;

    // scene()->views().first()->horizontalScroll();
    int hvalue = scene()->views().first()->horizontalScrollBar()->value();
    int vvalue = scene()->views().first()->verticalScrollBar()->value();

    qDebug() << "hvalue, vvalue = " << QPointF(hvalue, vvalue);

    return QRectF(leftTop, rightBot);
}


/*QRectF
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
}*/

void
GDALRasterItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_start_pose = event->screenPos();

    QGraphicsItem::mousePressEvent(event);
    update();
}

void GDALRasterItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        // m_cur_cursor_pose = event->pos();
        update();
        return;
    }

    QGraphicsItem::mouseMoveEvent(event);
    update();
}

void GDALRasterItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    m_cur_cursor_pose = event->screenPos();

    QRectF unity = scene()->views().first()->matrix().mapRect(QRectF(0, 0, 1, 1));
    double scale = unity.width();

    if(fabs(scale) < EPS)
        return;

    qDebug() << "GDALRasterItem::mouseReleaseEvent scale = " << scale;
    qDebug() << "GDALRasterItem::mouseReleaseEvent m_start_pose = " << m_start_pose;
    qDebug() << "GDALRasterItem::mouseReleaseEvent m_cur_cursor_pose = " << m_cur_cursor_pose;

    m_scale = scale;

    double dx_scene = m_start_pose.rx() - m_cur_cursor_pose.rx();
    double dy_scene = m_start_pose.ry() - m_cur_cursor_pose.ry();

    double dx_real = ovConst * (dx_scene); // / scale;
    double dy_real = ovConst * (dy_scene); // / scale;

    // double dx = ovConst * (event->lastScreenPos().rx() - event->screenPos().rx()) / scale;
    // double dy = ovConst * (event->lastScreenPos().ry() - event->screenPos().ry()) / scale;

    qDebug() << "GDALRasterItem::mouseReleaseEvent dx,dy = " << QPointF(dx_real, dy_real);

    m_real_move.setX(m_real_move.rx() - dx_real);
    m_real_move.setY(m_real_move.ry() - dy_real);

    qDebug() << "m_real_move = " << m_real_move;

    m_prev_pose = m_cur_cursor_pose;

    m_viewport_on_scene_matrix.translate(dx_scene, dy_scene);
    m_raster_matrix.translate(dx_real, dy_real);
    m_ov_matrix.translate(dx_scene, dy_scene);

    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void
GDALRasterItem::change_scale(double scale) {
    m_viewport_on_scene_matrix.scale(1 / scale, 1 / scale);
    m_raster_matrix.scale(scale, scale);
    m_ov_matrix.scale(scale, scale);
}
