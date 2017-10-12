#ifndef KREST_H
#define KREST_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>
#include <iostream>

class Krest : public QGraphicsItem
{
    Q_DECLARE_TR_FUNCTIONS(Krest)

public:;
	Krest(QGraphicsItem * parent = 0);
	~Krest();

	void setText(const QString &text);
	QString text();
	int X, Y;
	QPointF coord;
	QRectF boundingRect() const;
	// QPainterPath shape() const;
	void paint(QPainter *painter,
	           const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	// void mousePressEvent(QGraphicsSceneMouseEvent *event);
	// QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:;
	QRectF outlineRect() const;
	QString myText;
	QColor myTextColor;
	QColor myBackgroundColor;
	QColor myOutlineColor;
	QColor myKrestColor;
};

#endif
