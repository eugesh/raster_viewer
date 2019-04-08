#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image-view.h"
#include "image-item.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_scene = new QGraphicsScene(this);
    // ui->graphicsView->setScene(m_scene);


    m_item = new ImageItem();
    m_scene->addItem(m_item);

    ImageView *view = new ImageView("Top left view");
    view->view()->setScene(m_scene);

    setCentralWidget(view);

    connect(ui->actionOpen_image, SIGNAL(triggered()), this, SLOT(openImage()));
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

    m_item->setImage(m_image);

    return 0;
}

int
MainWindow::saveImageAs(QString path) {

    return 0;
}
