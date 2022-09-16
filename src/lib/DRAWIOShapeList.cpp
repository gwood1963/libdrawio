/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "DRAWIOShapeList.h"

namespace libdrawio {
  void DRAWIOShapeList::draw(librevenge::RVNGDrawingInterface *painter) {
    for (auto shape : shapes) {
      shape.draw(painter);
    }
  }

  void DRAWIOShapeList::append(MXCell cell) {
    shapes.push_back(cell);
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
