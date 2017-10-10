#ifndef KREST_H
#define KREST_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>
#include <iostream>

class Krest : public QGraphicsItem
{
    Q_DECLARE_TR_FUNCTIONS (Krest)

public:
	// int KrestType;
	Krest (QGraphicsItem * parent = 0);
	~Krest ();

	void setText (const QString &text);
	QString text ();
	//    QString getText() {return myText};
	int X, Y;
	QPointF coord;
	// void setTextColor(const QColor &color);
	// QColor textColor() const;
	// void setOutlineColor(const QColor &color);
	// QColor outlineColor() const;
	// void setBackgroundColor(const QColor &color);
	// QColor backgroundColor() const;

	// void addLink(Link *link);
	// void removeLink(Link *link);

	QRectF boundingRect () const;
	//QPainterPath shape () const;
	void paint (QPainter *painter,
	           const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
	void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event);
	// void mousePressEvent      (QGraphicsSceneMouseEvent *event);
	// QVariant itemChange(GraphicsItemChange change,
	//                        const QVariant &value);

private:
	QRectF outlineRect () const;
	// int roundness(double size) const;

	// QSet<Link *> myLinks;
	QString myText;
	QColor myTextColor;
	QColor myBackgroundColor;
	QColor myOutlineColor;
	QColor myKrestColor;
};

#endif
