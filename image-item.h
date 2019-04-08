#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>


class ImageItem : public QGraphicsItem
{
public:
    ImageItem();

    QRectF boundingRect() const override;
    //QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=nullptr) override;

    // temporal method.
    void setImage(QImage img) {
        m_image_part2draw = img;
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QImage m_image;
    QImage m_image_part2draw;
    QPointF cur_pose;

};

#endif // IMAGEITEM_H
