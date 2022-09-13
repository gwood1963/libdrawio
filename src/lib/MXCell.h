/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef MXCELL_H
#define MXCELL_H

#include "MXGeometry.h"
#include "DRAWIOUserObject.h"
#include <string>
#include <vector>

namespace libdrawio {
  struct MXCell {
    std::string id;
    DRAWIOUserObject data;
    MXGeometry geometry;
    std::string style;
    bool vertex, edge, connectable, visible, collapsed;
    std::string parent, source, target;
    std::vector<std::string> children; // holds references to child cells
    std::vector<std::string> edges; // holds references to connected edges
    MXCell()
      : id(), data(), geometry(), style(), vertex(), edge(), connectable(),
        visible(), collapsed(), parent(), source(), target(), children(),
        edges() {}
    MXCell(const MXCell &mxcell) = default;
    MXCell &operator=(const MXCell &mxcell) = default;
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
