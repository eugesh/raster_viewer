#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "image-item.h"
#include "gdal-raster-item.h"
#include "image-view.h"


QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QSplitter;
QT_END_NAMESPACE


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    int openImage();
    int openRaster();
    int saveImageAs(QString path);
    void change_scale_for_items(double);

private:
    void ini_settings();

public:
    ImageView      *m_view;
    GDALRasterItem *m_raster_item;
private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
    QImage          m_image;
    QString         m_img_path;
    ImageItem      *m_image_item;
    QString         m_raster_dir;
};

#endif // MAINWINDOW_H
