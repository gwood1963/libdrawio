/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef MXGEOMETRY_H
#define MXGEOMETRY_H

#include <vector>

namespace libdrawio {
  struct MXPoint {
    int x, y; // drawio only allows integer coordinates/lengths
    MXPoint() : x(), y() {}
    MXPoint(const MXPoint &mxpoint) = default;
    MXPoint &operator=(const MXPoint &mxpoint) = default;
  };

  struct MXGeometry {
    int x, y, width, height; // drawio only allows integer coordinates/lengths
    MXPoint sourcePoint, targetPoint;
    std::vector<MXPoint> points;
    int offset;
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
