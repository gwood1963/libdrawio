/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "DRAWIOPage.h"
#include "librevenge/librevenge.h"

namespace libdrawio {
  void DRAWIOPage::draw(librevenge::RVNGDrawingInterface *painter) {
    librevenge::RVNGPropertyList propList;
    propList.insert("svg:width", width / 100.);
    propList.insert("svg:height", height / 100.);
    propList.insert("draw:name", name);
    propList.insert("draw:id", id);
    painter->startPage(propList);
    elements.draw(painter);
  }

  void DRAWIOPage::insert(MXCell cell) {
    elements.append(cell);
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
