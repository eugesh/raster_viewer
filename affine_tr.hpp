#pragma once

#include <QRectF>
#include <QSize>

QRectF
scale_rect(QRectF rect_in, double scale_x, double scale_y) {
    QRectF rect_out = rect_in;

    double out_w = rect_in.width() * scale_x;
    double out_h = rect_in.height() * scale_y;
    double dx = (out_w - rect_in.width()) / 2;
    double dy = (out_h - rect_in.height()) / 2;

    rect_out.translate(dx, dy);
    rect_out.setHeight(out_h);
    rect_out.setWidth(out_w);

    return rect_out;
}


/*
 * Fits in size with mode and return size.
 */
QSize
fit_in_view(QSize const& view_size, QSize const& raster_size, Qt::AspectRatioMode aspectRatioMode=Qt::KeepAspectRatio) {
    QSize out_raster_size;
    //int raster_size;

    float x_ratio = float (view_size.width()) / raster_size.width();
    float y_ratio = float (view_size.height()) / raster_size.height();

    switch (aspectRatioMode) {
    case Qt::KeepAspectRatio:
        x_ratio = y_ratio = qMin(x_ratio, y_ratio);
        break;
    case Qt::KeepAspectRatioByExpanding:
        x_ratio = y_ratio = qMax(x_ratio, y_ratio);
        break;
    case Qt::IgnoreAspectRatio:
        break;
    }

    out_raster_size.setWidth(raster_size.width() * x_ratio);
    out_raster_size.setHeight(raster_size.height() * y_ratio);

    return out_raster_size;
}

QRect
recalc_view_raster_part(QSize view_size, QSize raster_size, QPoint center, double scale) {
    QRect ROI;

    double lshift, rshift, tshift, bshift; // Distances from center to view borders



    return ROI;
}
