/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "MXCell.h"
#include "DRAWIOTypes.h"
#include "librevenge/librevenge.h"
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <cmath>
#include <map>
#include <ostream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

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
    it = style_m.find("sourcePortConstraint"); if (it != style_m.end()) {
      if (it->second == "north") style.sourcePortConstraint = NORTH;
      else if (it->second == "east") style.sourcePortConstraint = EAST;
      else if (it->second == "south") style.sourcePortConstraint = SOUTH;
      else if (it->second == "west") style.sourcePortConstraint = WEST;
    }
    it = style_m.find("targetPortConstraint"); if (it != style_m.end()) {
      if (it->second == "north") style.targetPortConstraint = NORTH;
      else if (it->second == "east") style.targetPortConstraint = EAST;
      else if (it->second == "south") style.targetPortConstraint = SOUTH;
      else if (it->second == "west") style.targetPortConstraint = WEST;
    }
    it = style_m.find("PortConstraint"); if (it != style_m.end()) {
      if (it->second == "north") style.portConstraint = NORTH;
      else if (it->second == "east") style.portConstraint = EAST;
      else if (it->second == "south") style.portConstraint = SOUTH;
      else if (it->second == "west") style.portConstraint = WEST;
    }
  }

  void MXCell::setEndPoints(std::map<librevenge::RVNGString, MXCell> id_map) {
    if (!edge) return;
    if (!source_id.empty()) {
      MXCell source = id_map[source_id];
      if (!style.exitX.has_value() && !style.exitY.has_value()) {
        if (!target_id.empty()) {
          MXCell target = id_map.at(target_id);
          style.exitX =
            (source.geometry.x + source.geometry.width < target.geometry.x) ? 1 :
            (source.geometry.x > target.geometry.x + target.geometry.width) ? 0 : 0.5;
          if (style.exitX == 0.5)
            style.exitY = (source.geometry.y < target.geometry.y) ? 1 : 0;
          else style.exitY = 0.5;
        } else { // target point specified
          style.exitX =
            (source.geometry.x < geometry.targetPoint.x) ? 1 :
            (source.geometry.x > geometry.targetPoint.x) ? 0 : 0.5;
          if (style.exitX == 0.5)
            style.exitY = (source.geometry.y < geometry.targetPoint.y) ? 1 : 0;
          else style.exitY = 0.5;
        }
      }
      geometry.sourcePoint.x =
        source.geometry.x + (style.exitX.get() * source.geometry.width);
      geometry.sourcePoint.y =
        source.geometry.y + (style.exitY.get() * source.geometry.height);
    }
    if (!target_id.empty()) {
      MXCell target = id_map[target_id];
      if (!style.entryX.has_value() && !style.entryY.has_value()) {
        if (!source_id.empty()) {
          MXCell source = id_map.at(source_id);
          style.entryX =
            (target.geometry.x + target.geometry.width < source.geometry.x) ? 1 :
            (target.geometry.x > source.geometry.x + source.geometry.width) ? 0 : 0.5;
          if (style.entryX == 0.5)
            style.entryY = (target.geometry.y < source.geometry.y) ? 1 : 0;
          else style.entryY = 0.5;
        } else { // source point specified
          style.entryX =
            (target.geometry.x < geometry.sourcePoint.x) ? 1 :
            (target.geometry.x > geometry.sourcePoint.x) ? 0 : 0.5;
          if (style.entryX == 0.5)
            style.entryY = (target.geometry.y < geometry.sourcePoint.y) ? 1 : 0;
          else style.entryY = 0.5;
        }
      }
      geometry.targetPoint.x =
        target.geometry.x + (style.entryX.get() * target.geometry.width);
      geometry.targetPoint.y =
        target.geometry.y + (style.entryY.get() * target.geometry.height);
    }
  }
} // namespace libdrawio

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
