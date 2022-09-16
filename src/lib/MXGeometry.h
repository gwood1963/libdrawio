/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef MXGEOMETRY_H
#define MXGEOMETRY_H

#include <vector>

namespace libdrawio {
  struct MXPoint {
    double x, y;
    MXPoint() : x(), y() {}
    MXPoint(double x, double y) : x(x), y(y) {}
    MXPoint(const MXPoint &mxpoint) = default;
    MXPoint &operator=(const MXPoint &mxpoint) = default;
  };

  struct MXGeometry {
    double x, y, width, height;
    MXPoint sourcePoint, targetPoint;
    std::vector<MXPoint> points;
    double offset;
    bool relative;
    MXGeometry()
      : x(), y(), width(), height(), sourcePoint(), targetPoint(),
        points(), offset(), relative(false) {}
    MXGeometry(const MXGeometry &mxgeometry) = default;
    MXGeometry &operator=(const MXGeometry &mxgeometry) = default;
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
