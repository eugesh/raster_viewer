#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QWheelEvent>

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class ImageView;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(ImageView *v) : QGraphicsView(), view(v) { }
    //void fitInView(const QRectF &rect, Qt::AspectRatioMode aspectRadioMode = Qt::IgnoreAspectRatio);

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
#endif

private:
    ImageView *view;
};


class ImageView : public QFrame
{
    Q_OBJECT
public:
    explicit ImageView(const QString &name, QWidget *parent = nullptr);

    QGraphicsView *view() const;

signals:
    void scaleChanged(double value);

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePointerMode();
    void toggleOpenGL();
    void toggleAntialiasing();
    void print();
    void rotateLeft();
    void rotateRight();
private:
    GraphicsView *graphicsView;
    QLabel *label;
    QLabel *label2;
    QToolButton *selectModeButton;
    QToolButton *dragModeButton;
    QToolButton *openGlButton;
    QToolButton *antialiasButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;

};

#endif // IMAGEVIEW_H
