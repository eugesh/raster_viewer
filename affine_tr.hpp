#ifndef AFFINE_TR_HPP
#define AFFINE_TR_HPP

#include <QRectF>

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

#endif // AFFINE_TR_HPP
