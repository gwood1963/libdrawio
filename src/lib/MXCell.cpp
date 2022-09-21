/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "MXCell.h"
#include "librevenge/librevenge.h"
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <cmath>
#include <map>
#include <string>
#include <sstream>
#include <vector>

namespace libdrawio {

  void MXCell::draw(librevenge::RVNGDrawingInterface *painter,
                    std::map<librevenge::RVNGString, MXCell> id_map) {
  if (edge) {
    setEndPoints(id_map);
    calculateBounds();
    librevenge::RVNGPropertyList propList;
    if (!id.empty()) {
      propList.insert("draw:id", id);
      propList.insert("xml:id", id);
    }
    if (!source_id.empty()) {
      propList.insert("draw:start-shape", source_id);
    }
    if (!target_id.empty()) {
      propList.insert("draw:end-shape", target_id);
    }
    propList.insert("svg:x1", geometry.sourcePoint.x / 100.);
    propList.insert("svg:y1", geometry.sourcePoint.y / 100.);
    propList.insert("svg:x2", geometry.targetPoint.x / 100.);
    propList.insert("svg:y2", geometry.targetPoint.y / 100.);

    propList.insert("svg:d", getPath());

    propList.insert("draw:display", "always");

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

  void MXCell::calculateBounds() {
    if (vertex)
      bounds = {0, 0, 21600, 21600};
    else {
      int x = (int)std::min(geometry.sourcePoint.x, geometry.targetPoint.x);
      int y = (int)std::min(geometry.sourcePoint.y, geometry.targetPoint.y);
      int width =
        std::abs((int)(geometry.sourcePoint.x - geometry.targetPoint.x));
      int height =
        std::abs((int)(geometry.sourcePoint.y - geometry.targetPoint.y));
      bounds = {x, y, width, height};
    }
  }

  std::string MXCell::getViewBox() {
    std::ostringstream out;
    out << bounds.x << ' ' << bounds.y << ' '
        << bounds.width << ' ' << bounds.height;
    return out.str();
  }

  librevenge::RVNGPropertyListVector MXCell::getPath() {
    librevenge::RVNGPropertyListVector out;
    librevenge::RVNGPropertyList i;
    i.insert("librevenge:path-action", "M");
    i.insert("svg:x", geometry.sourcePoint.x / 100.);
    i.insert("svg:y", geometry.sourcePoint.y / 100.);
    out.append(i);
    i.clear();
    i.insert("librevenge:path-action", "L");
    i.insert("svg:x", geometry.targetPoint.x / 100.);
    i.insert("svg:y", geometry.targetPoint.y / 100.);
    out.append(i);
    return out;
  }

  void MXCell::setStyle() {
    std::string style_s = style_str.cstr();
    std::vector<std::string> tokens;
    boost::algorithm::split(tokens, style_s, boost::is_any_of(";"));
    std::map<std::string, std::string> style_m;
    for (auto t : tokens) {
      auto n = t.find('=');
      style_m[t.substr(0, n)] = n <= t.size() ? t.substr(n+1) : "";
    }
    auto it = style_m.find("entryX"); if (it != style_m.end())
      style.entryX = std::stod(it->second);
    it = style_m.find("entryY"); if (it != style_m.end())
      style.entryY = std::stod(it->second);
    it = style_m.find("exitX"); if (it != style_m.end())
      style.exitX = std::stod(it->second);
    it = style_m.find("exitY"); if (it != style_m.end())
      style.exitY = std::stod(it->second);
  }

  void MXCell::setEndPoints(std::map<librevenge::RVNGString, MXCell> id_map) {
    if (!edge) return;
    if (!source_id.empty()) {
      MXCell source = id_map[source_id];
      geometry.sourcePoint.x =
        source.geometry.x + (style.exitX * source.geometry.width);
      geometry.sourcePoint.y =
        source.geometry.y + (style.exitY * source.geometry.height);
    }
    if (!target_id.empty()) {
      MXCell target = id_map[target_id];
      geometry.targetPoint.x =
        target.geometry.x + (style.entryX * target.geometry.width);
      geometry.sourcePoint.y =
        target.geometry.y + (style.entryY * target.geometry.height);
    }
  }
} // namespace libdrawio

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
