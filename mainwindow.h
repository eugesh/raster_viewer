#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "image-item.h"
#include "image-processing-common.h"


QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QSplitter;
class QDockWidget;
class ImageView;
QT_END_NAMESPACE


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void create4Windows();
    void openImages4Windows();
    void show4Windows();
    void hide4Windows();
    // void changeColorSpace(QColor::Spec s);
    void changeColorSpace(ColorSpace s);

signals:
    void scaleChanged(double);
    void angleChanged(double);
    void imagePlaceChanged(double, double);

private slots:
    int openImage();
    int saveImageAs(const QString &path);
    void onAddVSplitter();
    void onAddHSplitter();
    void onAddDockWidget();
    void on4WindowsCheck(int check);
    void onActionRGB();
    void onActionHSV();
    void onActionHSI();
    //void onSimultaneousScrollCheck(int check);

private:
    Ui::MainWindow *ui;
    // Main Scene
    QGraphicsScene *m_scene;
    ImageView      *m_view;
    QImage          m_image;
    QString         m_img_path;
    ImageItem      *m_item;
    //
    QVector<QSharedPointer<QSplitter> > m_vpSplitters;
    // Scenes created after splitters
    // QVector<QSharedPointer<QGraphicsScene> > m_vpScene;
    QVector<QSharedPointer<ImageView> > m_vpImageView;
    QVector<QSharedPointer<ImageItem> > m_vpImageItems;
    bool m_isSimultaneousScroll = true;
    ColorSpace m_colorSpace = RGB;
    QString m_lastPath = "/home";
};

#endif // MAINWINDOW_H
