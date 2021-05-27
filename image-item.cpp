#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include "image-item.h"

ImageItem::ImageItem()
{
    setZValue(100);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF
ImageItem::boundingRect() const {
    return QRectF(QPointF(0, 0), m_image_part2draw.size());
}

/*QPainterPath
ImageItem::shape() const
{
    QPainterPath path;
    path.addRect(14, 14, 82, 42);
    return path;
}*/

void
ImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget)
    Q_UNUSED(option)
    // painter->drawImage(QPoint(0,0), m_image_part2draw);
    painter->drawImage(cur_pose, m_image_part2draw);
}

QImage ImageItem::setFiltered(const QImage &img)
{
    m_image_part2draw = img; return m_image_part2draw;
}

void ImageItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void ImageItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        cur_pose = event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void ImageItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
