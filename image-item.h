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
    void setImage(const QImage &img) {
        m_image = img;
        m_image_part2draw = img;
    }
    QImage setFiltered(const QImage &img);
    QImage getImage() const { return m_image; }
    QImage getFiltered() const { return m_image_part2draw; }
    // QImage applyFilterOnImage();
    // QImage applyFilterOnFiltered();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    // Original
    QImage m_image;
    // Filtered
    QImage m_image_part2draw;
    QPointF cur_pose;

};

#endif // IMAGEITEM_H
