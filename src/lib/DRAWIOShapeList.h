/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef DRAWIOSHAPELIST_H
#define DRAWIOSHAPELIST_H

#include "MXCell.h"
#include "librevenge/RVNGDrawingInterface.h"
#include <vector>

namespace libdrawio {
  class DRAWIOShapeList {
  public:
    DRAWIOShapeList() : shapes() {}
    DRAWIOShapeList(const DRAWIOShapeList &list) = default;
    DRAWIOShapeList &operator=(const DRAWIOShapeList &list) = default;
    void append(MXCell cell);
    void draw(librevenge::RVNGDrawingInterface *painter);
  private:
    std::vector<MXCell> shapes;
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
