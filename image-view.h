#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QWheelEvent>
#include "image-processing-common.h"

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
    ColorSpace colorSpace() { return m_colorSpace; }
    int channelNumber() { return m_channelNumber; }
    void changeColorSpace(ColorSpace cs) { m_colorSpace = cs; }
    void changeChannelNumber(int cn) { m_channelNumber = cn; }

signals:
    void scaleChanged(double value);
    void angleChanged(double value);

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);
    void rotate(double angleStep);

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
    ColorSpace m_colorSpace;
    int m_channelNumber;
};

#endif // IMAGEVIEW_H
