#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "image-item.h"


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
    int saveImageAs(QString path);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
    QImage          m_image;
    QString         m_img_path;
    ImageItem      *m_item;
};

#endif // MAINWINDOW_H
