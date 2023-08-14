/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "MXCell.h"
#include "DRAWIOTypes.h"
#include "libdrawio_xml.h"
#include "librevenge/RVNGPropertyList.h"
#include "librevenge/RVNGPropertyListVector.h"
#include "librevenge/RVNGString.h"
#include "librevenge/librevenge.h"
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/none.hpp>
#include <cmath>
#include <map>
#include <ostream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

namespace libdrawio {
  int MXCell::draw_count = 0;

  void MXCell::draw(librevenge::RVNGDrawingInterface *painter,
                    std::map<librevenge::RVNGString, MXCell> id_map) {
    librevenge::RVNGPropertyList propList;
    if (!id.empty()) {
      propList.insert("draw:id", id);
      propList.insert("xml:id", id);
    }
    std::string style_name = "gr_" + std::to_string(draw_count);
    librevenge::RVNGPropertyList styleProps = getStyle();
    styleProps.insert("style:display-name", style_name.c_str());
    propList.insert("draw:style-name", style_name.c_str());
    painter->setStyle(styleProps);
    if (edge) {
      setEndPoints(id_map);
      calculateBounds();
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

      painter->drawConnector(propList);
    } else if (vertex) {
      propList.insert("svg:x", geometry.x / 100.);
      propList.insert("svg:y", geometry.y / 100.);
      propList.insert("svg:width", geometry.width / 100.);
      propList.insert("svg:height", geometry.height / 100.);
      if (style.shape == RECTANGLE)
        painter->drawRectangle(propList);
      else if (style.shape == ELLIPSE)
        painter->drawEllipse(propList);
      else if (style.shape == TRIANGLE) {
        librevenge::RVNGPropertyListVector points;
        librevenge::RVNGPropertyList point;
        switch (style.direction) {
        case NORTH:
          point.insert("svg:x", (geometry.x + geometry.width / 2) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          break;
        case EAST:
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height / 2) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          break;
        case SOUTH:
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width / 2) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          break;
        case WEST:
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height / 2) / 100.);
          points.append(point); point.clear();
          break;
        }
        propList.insert("svg:points", points);
        painter->drawPolygon(propList);
      } else if (style.shape == CALLOUT) {
        librevenge::RVNGPropertyListVector points;
        librevenge::RVNGPropertyList point;
        switch (style.direction) {
        case NORTH:
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width - style.calloutLength) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width - style.calloutLength) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height * style.calloutPosition - style.calloutWidth) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height * style.calloutTipPosition) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width - style.calloutLength) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height * style.calloutPosition) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width - style.calloutLength) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          break;
        case EAST:
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height - style.calloutLength) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width * style.calloutPosition + style.calloutWidth) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height - style.calloutLength) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width * style.calloutTipPosition) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width * style.calloutPosition) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height - style.calloutLength) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height - style.calloutLength) / 100.);
          points.append(point); point.clear();
          break;
        case SOUTH:
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + style.calloutLength) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + style.calloutLength) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height * style.calloutPosition) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height * style.calloutTipPosition) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + style.calloutLength) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height * style.calloutPosition + style.calloutWidth) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + style.calloutLength) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          break;
        case WEST:
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y + geometry.height) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width) / 100.);
          point.insert("svg:y", (geometry.y + style.calloutLength) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width * style.calloutPosition) / 100.);
          point.insert("svg:y", (geometry.y + style.calloutLength) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width * style.calloutTipPosition) / 100.);
          point.insert("svg:y", (geometry.y) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x + geometry.width * style.calloutPosition - style.calloutWidth) / 100.);
          point.insert("svg:y", (geometry.y + style.calloutLength) / 100.);
          points.append(point); point.clear();
          point.insert("svg:x", (geometry.x) / 100.);
          point.insert("svg:y", (geometry.y + style.calloutLength) / 100.);
          points.append(point); point.clear();
          break;
        }
        propList.insert("svg:points", points);
        painter->drawPolygon(propList);
      } else if (style.shape == PROCESS) {
        librevenge::RVNGPropertyListVector path;
        librevenge::RVNGPropertyList element;
        element.insert("librevenge:path-action", "M");
        element.insert("svg:x", (geometry.x) / 100.);
        element.insert("svg:y", (geometry.y) / 100.);
        path.append(element); element.clear();
        element.insert("librevenge:path-action", "L");
        element.insert("svg:x", (geometry.x + geometry.width) / 100.);
        element.insert("svg:y", (geometry.y) / 100.);
        path.append(element); element.clear();
        element.insert("librevenge:path-action", "L");
        element.insert("svg:x", (geometry.x + geometry.width) / 100.);
        element.insert("svg:y", (geometry.y + geometry.height) / 100.);
        path.append(element); element.clear();
        element.insert("librevenge:path-action", "L");
        element.insert("svg:x", (geometry.x) / 100.);
        element.insert("svg:y", (geometry.y + geometry.height) / 100.);
        path.append(element); element.clear();
        element.insert("librevenge:path-action", "Z");
        path.append(element); element.clear();
        switch (style.direction) {
        case NORTH:
        case SOUTH:
          element.insert("librevenge:path-action", "M");
          element.insert("svg:x", (geometry.x) / 100.);
          element.insert("svg:y",
                         (geometry.y + geometry.height * style.processBarSize) / 100.);
          path.append(element); element.clear();
          element.insert("librevenge:path-action", "L");
          element.insert("svg:x", (geometry.x + geometry.width) / 100.);
          element.insert("svg:y",
                         (geometry.y + geometry.height * style.processBarSize) / 100.);
          path.append(element); element.clear();
          element.insert("librevenge:path-action", "M");
          element.insert("svg:x", (geometry.x) / 100.);
          element.insert("svg:y",
                         (geometry.y + geometry.height * (1 - style.processBarSize)) / 100.);
          path.append(element); element.clear();
          element.insert("librevenge:path-action", "L");
          element.insert("svg:x", (geometry.x + geometry.width) / 100.);
          element.insert("svg:y",
                         (geometry.y + geometry.height * (1 - style.processBarSize)) / 100.);
          path.append(element); element.clear();
          break;
        case EAST:
        case WEST:
          element.insert("librevenge:path-action", "M");
          element.insert("svg:x",
                         (geometry.x + geometry.width * style.processBarSize) / 100.);
          element.insert("svg:y", (geometry.y) / 100.);
          path.append(element); element.clear();
          element.insert("librevenge:path-action", "L");
          element.insert("svg:x",
                         (geometry.x + geometry.width * style.processBarSize) / 100.);
          element.insert("svg:y", (geometry.y + geometry.height) / 100.);
          path.append(element); element.clear();
          element.insert("librevenge:path-action", "M");
          element.insert("svg:x",
                         (geometry.x + geometry.width * (1 - style.processBarSize)) / 100.);
          element.insert("svg:y", (geometry.y) / 100.);
          path.append(element); element.clear();
          element.insert("librevenge:path-action", "L");
          element.insert("svg:x",
                         (geometry.x + geometry.width * (1 - style.processBarSize)) / 100.);
          element.insert("svg:y", (geometry.y + geometry.height) / 100.);
          path.append(element); element.clear();
          break;
        }
        propList.insert("svg:d", path);
        painter->drawPath(propList);
      }
    }
    if (!data.label.empty()) {
      painter->startTextObject(propList);
      painter->openParagraph(propList);
      painter->insertText(processText(data.label));
      painter->closeParagraph();
      painter->endTextObject();
    }
    draw_count++;
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
    it = style_m.find("ellipse"); if (it != style_m.end()) style.shape = ELLIPSE;
    it = style_m.find("triangle"); if (it != style_m.end()) style.shape = TRIANGLE;
    it = style_m.find("shape"); if (it != style_m.end()) {
      if (it->second == "callout") style.shape = CALLOUT;
      else if (it->second == "process") style.shape = PROCESS;
    }
    it = style_m.find("direction"); if (it != style_m.end()) {
      if (it->second == "north") style.direction = NORTH;
      else if (it->second == "east") style.direction = EAST;
      else if (it->second == "south") style.direction = SOUTH;
      else if (it->second == "west") style.direction = WEST;
    }
    it = style_m.find("size"); if (it != style_m.end()) {
      if (style.shape == CALLOUT) style.calloutLength = std::stod(it->second);
      else if (style.shape == PROCESS) style.processBarSize = std::stod(it->second);
    }
    it = style_m.find("base"); if (it != style_m.end()) {
      if (style.shape == CALLOUT) style.calloutWidth = std::stod(it->second);
    }
    it = style_m.find("position"); if (it != style_m.end()) {
      if (style.shape == CALLOUT) style.calloutPosition = std::stod(it->second);
    }
    it = style_m.find("position2"); if (it != style_m.end()) {
      if (style.shape == CALLOUT) style.calloutTipPosition = std::stod(it->second);
    }
    it = style_m.find("fillColor"); if (it != style_m.end()) {
      if (it->second == "none") style.fillColor = boost::none;
      else style.fillColor = xmlStringToColor((xmlChar*)(it->second.c_str()));
    }
    it = style_m.find("strokeColor"); if (it != style_m.end()) {
      if (it->second == "none") style.strokeColor = boost::none;
      else style.strokeColor = xmlStringToColor((xmlChar*)(it->second.c_str()));
    }
    it = style_m.find("startArrow"); if (it != style_m.end()) {
      if (it->second == "none") style.startArrow = boost::none;
      else if (it->second == "classic") style.startArrow = CLASSIC;
    }
    it = style_m.find("startFill"); if (it != style_m.end())
      style.startFill = xmlStringToBool((xmlChar*)(it->second.c_str()));
    it = style_m.find("startSize"); if (it != style_m.end())
      style.startSize = xmlStringToDouble((xmlChar*)(it->second.c_str()));
    it = style_m.find("endArrow"); if (it != style_m.end()) {
      if (it->second == "none") style.endArrow = boost::none;
      else if (it->second == "classic") style.endArrow = CLASSIC;
    }
    it = style_m.find("endFill"); if (it != style_m.end())
      style.endFill = xmlStringToBool((xmlChar*)(it->second.c_str()));
    it = style_m.find("endSize"); if (it != style_m.end())
      style.endSize = xmlStringToDouble((xmlChar*)(it->second.c_str()));
  }

  void MXCell::setEndPoints(std::map<librevenge::RVNGString, MXCell> id_map) {
    // calculates endpoints for an edge
    // necessary because draw.io doesn't store endpoint coordinates
    // if the edge is attached to a vertex.
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
        if (source.style.shape == TRIANGLE) {
          // update exitX, exitY to connect with diagonal edges of triangle
          switch (source.style.direction) {
          case NORTH:
          case SOUTH:
            style.exitX = (style.exitX.get() == 1) ? 0.75 : 0.25;
            break;
          case EAST:
          case WEST:
            style.exitY = (style.exitY.get() == 1) ? 0.75 : 0.25;
            break;
          }
        } else if (source.style.shape == CALLOUT) {
          if (source.style.direction == NORTH) {
            style.exitX =
              std::min(1 - (source.style.calloutLength / source.geometry.width), style.exitX.get());
          } else if (source.style.direction == EAST) {
            style.exitY =
              std::min(1 - (source.style.calloutLength / source.geometry.height), style.exitY.get());
          } else if (source.style.direction == SOUTH) {
            style.exitX =
              std::max(source.style.calloutLength / source.geometry.width, style.exitX.get());
          } else if (source.style.direction == WEST) {
            style.exitY =
              std::max(source.style.calloutLength / source.geometry.height, style.exitY.get());
          }
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
        if (target.style.shape == TRIANGLE) {
          // update entryX, entryY to connect with diagonal edges of triangle
          switch (target.style.direction) {
          case NORTH:
          case SOUTH:
            style.entryX = (style.entryX.get() == 1) ? 0.75 : 0.25;
            break;
          case EAST:
          case WEST:
            style.entryY = (style.entryY.get() == 1) ? 0.75 : 0.25;
            break;
          }
        } else if (target.style.shape == CALLOUT) {
          if (target.style.direction == NORTH) {
            style.entryX =
              std::min(1 - (target.style.calloutLength / target.geometry.width), style.entryX.get());
          } else if (target.style.direction == EAST) {
            style.entryY =
              std::min(1 - (target.style.calloutLength / target.geometry.height), style.entryY.get());
          } else if (target.style.direction == SOUTH) {
            style.entryX =
              std::max(target.style.calloutLength / target.geometry.width, style.entryX.get());
          } else if (target.style.direction == WEST) {
            style.entryY =
              std::max(target.style.calloutLength / target.geometry.height, style.entryY.get());
          }
        }
      }
      geometry.targetPoint.x =
        target.geometry.x + (style.entryX.get() * target.geometry.width);
      geometry.targetPoint.y =
        target.geometry.y + (style.entryY.get() * target.geometry.height);
    }
  }

  librevenge::RVNGString MXCell::processText(librevenge::RVNGString input) {
    librevenge::RVNGString out;
    bool skipping = false;
    for (unsigned long i = 0; i < input.size(); i++) {
      char c = input.cstr()[i];
      if (c == '<') skipping = true;
      if (!skipping) out.append(c);
      if (c == '>') skipping = false;
    }
    return out;
  }

  std::string MXCell::getMarkerViewBox(MarkerType marker) {
    switch (marker) {
    case CLASSIC:
      return "0 0 40 40";
    };
  }

  std::string MXCell::getMarkerPath(MarkerType marker) {
    switch (marker) {
    case CLASSIC:
      return "M 20 0 L 40 40 L 20 30 L 0 40 Z";
    }
  }

  librevenge::RVNGPropertyList MXCell::getStyle() {
    librevenge::RVNGPropertyList styleProps;
    if (!style.fillColor.has_value()) styleProps.insert("draw:fill", "none");
    else {
      styleProps.insert("draw:fill", "solid");
      styleProps.insert("draw:fill-color", style.fillColor->to_string().c_str());
    }
    if (!style.strokeColor.has_value()) styleProps.insert("draw:stroke", "none");
    else {
      styleProps.insert("draw:stroke", "solid");
      styleProps.insert("svg:stroke-color", style.strokeColor->to_string().c_str());
      if (edge && (style.endFill || style.startFill)) {
        styleProps.remove("draw:fill"); styleProps.remove("draw:fill-color");
        styleProps.insert("draw:fill", "solid");
        styleProps.insert("draw:fill-color", styleProps["svg:stroke-color"]->getStr());
      }
    }
    if (style.startArrow.has_value() && edge) {
      styleProps.insert("draw:marker-start-viewbox",
                        getMarkerViewBox(style.startArrow.get()).c_str());
      styleProps.insert("draw:marker-start-path",
                        getMarkerPath(style.startArrow.get()).c_str());
      styleProps.insert("draw:marker-start-width", style.startSize / 100.);
    }
    if (style.endArrow.has_value() && edge) {
      styleProps.insert("draw:marker-end-viewbox",
                        getMarkerViewBox(style.endArrow.get()).c_str());
      styleProps.insert("draw:marker-end-path",
                        getMarkerPath(style.endArrow.get()).c_str());
      styleProps.insert("draw:marker-end-width", style.endSize / 100.);
    }
    return styleProps;
  }
} // namespace libdrawio

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
