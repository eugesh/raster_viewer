#ifndef __GRAPHIC_ITEM_IMAGE_H__
#define __GRAPHIC_ITEM_IMAGE_H__

#include <gdal_priv.h> 

#include <QGraphicsItem>
#include <QString>
#include <QSize>
#include <QPainter>
#include <QFile>

//! класс, описвающий объект растр с использованием gdal для работы с технологией QGraphicsScene/GraphicsQView
class QGraphicsItem_Image : public QGraphicsItem {
private:;
	QString name;//!< имя файла с растровыми данными
	QSize  size_raster;//!< размеры растра в пикселях
	int number_bands;//!< количество bandов
	int list_bands[3];//!< список номеров bandов при формировании растровых данных
public:;
	//! конструктор
	QGraphicsItem_Image(QGraphicsItem * parent = 0);
	//! задание имени файла с растровыми данными
	void set_image(const QString &name_file);
	//! получение границ
	virtual QRectF	boundingRect () const;
	//! отрисовка
	virtual void	paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	//!
};
#endif
