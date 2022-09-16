/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef DRAWIOPAGE_H
#define DRAWIOPAGE_H

#include "DRAWIOShapeList.h"
#include "MXCell.h"
#include "librevenge/librevenge.h"
#include <string>

namespace libdrawio {
  class DRAWIOPage {
  public:
    DRAWIOPage() : name(""), id(""), width(0), height(0), elements() {}
    DRAWIOPage(const DRAWIOPage & page) = default;
    DRAWIOPage &operator=(const DRAWIOPage &page) = default;
    librevenge::RVNGString name, id;
    int width, height;
    void draw(librevenge::RVNGDrawingInterface *painter);
    void insert(MXCell cell);
  private:
    DRAWIOShapeList elements;
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
