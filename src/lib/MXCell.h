/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef MXCELL_H
#define MXCELL_H

#include "MXGeometry.h"
#include "DRAWIOUserObject.h"
#include "librevenge/librevenge.h"
#include <string>
#include <vector>

namespace libdrawio {
  struct MXCell {
    librevenge::RVNGString id;
    DRAWIOUserObject data;
    MXGeometry geometry;
    librevenge::RVNGString style;
    bool vertex, edge, connectable, visible, collapsed;
    librevenge::RVNGString parent, source, target;
    std::vector<librevenge::RVNGString> children; // holds references to child cells
    std::vector<librevenge::RVNGString> edges; // holds references to connected edges
    MXCell()
      : id(), data(), geometry(), style(), vertex(), edge(), connectable(),
        visible(), collapsed(), parent(), source(), target(), children(),
        edges() {}
    MXCell(const MXCell &mxcell) = default;
    MXCell &operator=(const MXCell &mxcell) = default;
    void draw(librevenge::RVNGDrawingInterface *painter);
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
