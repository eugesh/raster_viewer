#include <QDockWidget>
#include <QFileDialog>
#include <QSignalMapper>
#include <QSplitter>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image-view.h"
#include "image-item.h"
#include "imageprocessor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_scene = new QGraphicsScene(this);
    // ui->graphicsView->setScene(m_scene);


    m_item = new ImageItem();
    m_scene->addItem(m_item);

    m_view = new ImageView("Main view");
    m_view->view()->setScene(m_scene);

    setCentralWidget(m_view);

    connect(ui->actionOpen_image, &QAction::triggered, this, &MainWindow::openImage);
    //  ToDo: create shortcuts
    connect(ui->actionHorizontal, &QAction::triggered, this, &MainWindow::onAddHSplitter);
    connect(ui->actionVertical, &QAction::triggered, this, &MainWindow::onAddVSplitter);
    connect(ui->actionAddDockWidget, &QAction::triggered, this, &MainWindow::onAddDockWidget);
    // connect(ui->actionSimultaneousScroll, &QAction::chan, this, &MainWindow::onSimultaneousScrollCheck);
    connect(ui->action4Windows, &QAction::triggered, this, &MainWindow::on4WindowsCheck);
    connect(ui->actionRGB, &QAction::triggered, this, &MainWindow::onActionRGB);
    connect(ui->actionHSV, &QAction::triggered, this, &MainWindow::onActionHSV);
    connect(ui->actionHSI, &QAction::triggered, this, &MainWindow::onActionHSI);

    connect(m_view, &ImageView::scaleChanged, this, &MainWindow::scaleChanged);
    connect(m_view, &ImageView::angleChanged, this, &MainWindow::angleChanged);
    connect(this, &MainWindow::scaleChanged, m_view, &ImageView::zoomIn);
    connect(this, &MainWindow::angleChanged, m_view, &ImageView::rotate);
}

void MainWindow::on4WindowsCheck(int check)
{
    if (check) {
        if (m_vpSplitters.empty())
            create4Windows();
        else
            show4Windows();
    } else {
        hide4Windows();
    }
}

void MainWindow::onActionRGB()
{
    changeColorSpace(RGB);
    ui->actionHSV->setChecked(false);
    ui->actionHSI->setChecked(false);
}

void MainWindow::onActionHSV()
{
    changeColorSpace(HSV);
    ui->actionRGB->setChecked(false);
    ui->actionHSI->setChecked(false);
}

void MainWindow::onActionHSI()
{
    changeColorSpace(HSI);
    ui->actionHSV->setChecked(false);
    ui->actionRGB->setChecked(false);
}

/*void MainWindow::changeColorSpace(QColor::Spec s)
{
    m_colorSpace ;
}*/

void MainWindow::changeColorSpace(ColorSpace s)
{
    m_colorSpace = s;

    openImages4Windows();
}

void MainWindow::create4Windows()
{
    QSplitter *h1Splitter = new QSplitter(this);
    QSplitter *h2Splitter = new QSplitter(this);

    QSplitter *vSplitter = new QSplitter;
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->addWidget(h1Splitter);
    vSplitter->addWidget(h2Splitter);

    h1Splitter->addWidget(m_view);

    // QSharedPointer<ImageView> view (new ImageView("Top right view"));
    m_vpImageView.push_back(QSharedPointer<ImageView>(new ImageView("Top right view")));
    QGraphicsScene *scene = new QGraphicsScene(this);
    m_vpImageView.last()->view()->setScene(scene);
    h1Splitter->addWidget(m_vpImageView.last().get());
    //m_vpImageView.push_back(view);

    //view = new ImageView("Bottom left view");
    m_vpImageView.push_back(QSharedPointer<ImageView>(new ImageView("Bottom left view")));
    scene = new QGraphicsScene(this);
    m_vpImageView.last()->view()->setScene(scene);
    h2Splitter->addWidget(m_vpImageView.last().get());

    //view = new ImageView("Bottom right view");
    m_vpImageView.push_back(QSharedPointer<ImageView>(new ImageView("Bottom right view")));
    scene = new QGraphicsScene(this);
    m_vpImageView.last()->view()->setScene(scene);
    h2Splitter->addWidget(m_vpImageView.last().get());

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(vSplitter);
    setLayout(layout);
    setCentralWidget(vSplitter);

    //QSignalMapper mapper(this);
    // mapper.setMapping(m_view, 0);
    // connect(m_view, &ImageView::scaleChanged, &mapper, &QSignalMapper::map);
    // connect(m_view, SIGNAL(scaleChanged(double)), &mapper, SLOT(map()));
    for (int i = 0; i < m_vpImageView.size(); ++i) {
        // mapper.setMapping(m_vpImageView[i].get(), i + 1);
        // connect(m_vpImageView[i].get(), &ImageView::scaleChanged, &mapper, &QSignalMapper::map);
        // connect(m_vpImageView[i].get(), SIGNAL(scaleChanged(double)), &mapper, SLOT(map()));
        // connect(mapper, SIGNAL(mapped(int)), this, SLOT(yourSlot(int)));
        connect(m_vpImageView[i].get(), &ImageView::scaleChanged, this, &MainWindow::scaleChanged);
        connect(m_vpImageView[i].get(), &ImageView::angleChanged, this, &MainWindow::angleChanged);
        connect(this, &MainWindow::scaleChanged, m_vpImageView[i].get(), &ImageView::zoomIn);
        connect(this, &MainWindow::angleChanged, m_vpImageView[i].get(), &ImageView::rotate);
    }
}

void MainWindow::openImages4Windows()
{
    for (int i = 0; i < m_vpImageView.size(); ++i) {
        m_vpImageView[i]->changeColorSpace(m_colorSpace);
        m_vpImageView[i]->changeChannelNumber(i);
        if (m_vpImageItems.count() <= i) {
            m_vpImageItems.push_back(QSharedPointer<ImageItem>(new ImageItem()));
            m_vpImageItems[i]->setImage(m_item->getImage());
            QGraphicsScene *scene = new QGraphicsScene(this);
            scene->addItem(m_vpImageItems[i].get());
            m_vpImageView[i]->view()->setScene(scene);
        }
        m_vpImageItems[i]->setFiltered(getChannel(m_item->getImage(), m_colorSpace, i));
    }
}

void MainWindow::show4Windows()
{

}

void MainWindow::hide4Windows()
{

}

// ui->actionSimultaneousScroll->isChecked();
/*void MainWindow::onSimultaneousScrollCheck(int check) {
    m_isSimultaneousScroll = check;
}*/

MainWindow::~MainWindow()
{
    delete ui;
}

int
MainWindow::openImage() {
    QString fullFilePath = QFileDialog::getOpenFileName(this, tr("Choose image file"), m_lastPath, tr("Images (*.png *.bmp *.tif *.xpm *.jpg *.jpeg *.JPG)"));
    // m_settings_dlg->ui->queue_lineEdit->setText(queuePath);

    if (fullFilePath.isEmpty())
        return -1;

    QFileInfo fi(fullFilePath);
    m_lastPath = fi.absoluteDir().path();

    m_image = QImage(fullFilePath);

    m_item->setImage(m_image);

    openImages4Windows();

    return 0;
}

int MainWindow::saveImageAs(const QString &path) {

    return 0;
}

void MainWindow::onAddVSplitter() {
    QSplitter *vSplitter = new QSplitter;
    vSplitter->setOrientation(Qt::Vertical);

}

void MainWindow::onAddHSplitter() {

}

void MainWindow::onAddDockWidget() {
    QDockWidget *dock = new QDockWidget(tr(""), this);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}
