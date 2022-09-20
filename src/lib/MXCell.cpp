/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "MXCell.h"
#include "librevenge/RVNGDrawingInterface.h"
#include "librevenge/RVNGPropertyList.h"
#include "librevenge/RVNGString.h"

namespace libdrawio {
  void MXCell::draw(librevenge::RVNGDrawingInterface *painter) {
    if (edge) {
      librevenge::RVNGPropertyList propList;
      if (!id.empty()) {
        propList.insert("draw:id", id);
        propList.insert("xml:id", id);
      }
      if (!source.empty()) {
        propList.insert("draw:start-shape", source);
      }
      if (!target.empty()) {
        propList.insert("draw:end-shape", target);
      }
      propList.insert("svg:x1", geometry.sourcePoint.x / 100.);
      propList.insert("svg:y1", geometry.sourcePoint.y / 100.);
      propList.insert("svg:x2", geometry.targetPoint.x / 100.);
      propList.insert("svg:y2", geometry.targetPoint.y / 100.);
      painter->drawConnector(propList);
    } else if (vertex) {
      librevenge::RVNGPropertyList propList;
      if (!id.empty()) {
        propList.insert("draw:id", id);
        propList.insert("xml:id", id);
      }
      propList.insert("svg:x", geometry.x / 100.);
      propList.insert("svg:y", geometry.y / 100.);
      propList.insert("svg:width", geometry.width / 100.);
      propList.insert("svg:height", geometry.height / 100.);
      painter->drawRectangle(propList);
    }
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
