#include <QFileDialog>
#include <QGraphicsView>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image-item.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ini_settings();

    m_scene = new QGraphicsScene(this);
    // ui->graphicsView->setScene(m_scene);


    m_image_item = new ImageItem();
    m_scene->addItem(m_image_item);

    m_view = new ImageView("Top left view");
    m_view->view()->setScene(m_scene);

    setCentralWidget(m_view);

    connect(ui->actionOpen_image, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(ui->actionOpen_raster, SIGNAL(triggered()), this, SLOT(openRaster()));
}

void
MainWindow::ini_settings() {
    m_raster_item = nullptr;
    m_image_item = nullptr;

    m_raster_dir = tr("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

int
MainWindow::openImage() {
    // QString path = QOpenFileDialog;
    QString path = QFileDialog::getOpenFileName(this, "Choose image file", tr("Image (*.png, *.bmp, *.tif)"));
    // m_settings_dlg->ui->queue_lineEdit->setText(queuePath);

    m_image = QImage(path);

    m_image_item->setImage(m_image);

    return 0;
}

int
MainWindow::openRaster() {
    if(! m_raster_item)
        m_raster_item = new GDALRasterItem();

    //  Dialog to open filename;
    QString path_to_raster =
            QFileDialog::getOpenFileName(this, tr("OPen raster"), m_raster_dir);

    m_scene->addItem(m_raster_item);
    // Create GDAL raster;
    // GDALWrapper *raster = new GDALWrapper();

    // Set raster to raster item;
    m_raster_item->setRaster(path_to_raster);

    m_view->view()->fitInView(m_raster_item->boundingRect(), Qt::KeepAspectRatio);

    return 0;
}

int
MainWindow::saveImageAs(QString path) {

    return 0;
}
