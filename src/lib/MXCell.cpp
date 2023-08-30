/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "MXCell.h"
#include "DRAWIOTypes.h"
#include "libdrawio_xml.h"
#include "librevenge/RVNGPropertyList.h"
#include "librevenge/RVNGPropertyListVector.h"
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/none.hpp>
#include <cmath>
#include <initializer_list>
#include <map>
#include <ostream>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <iostream>
#include <librevenge/librevenge.h>

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
    }
    else if (vertex) {
      double x, y;
      double rx = geometry.width / 200.; double ry = geometry.height / 200.;
      double cx = geometry.x / 100. + rx; double cy = geometry.y / 100. + ry;
      double angle = -style.rotation * boost::math::double_constants::pi / 180;
      if (style.shape == RECTANGLE) {
        propList.insert("svg:x", geometry.x / 100.);
        propList.insert("svg:y", geometry.y / 100.);
        propList.insert("svg:width", geometry.width / 100.);
        propList.insert("svg:height", geometry.height / 100.);
        double dx = sqrt(pow(rx, 2.) + pow(ry, 2.))*cos(atan(ry/rx)-angle) - rx;
        double dy = sqrt(pow(rx, 2.) + pow(ry, 2.))*sin(atan(ry/rx)-angle) - ry;
        librevenge::RVNGString sValue = "translate(";
        sValue.append(std::to_string(-geometry.x / 100.).c_str());
        sValue.append("in,");
        sValue.append(std::to_string(-geometry.y / 100.).c_str());
        sValue.append("in) rotate(");
        sValue.append(std::to_string(angle).c_str());
        sValue.append(") translate(");
        sValue.append(std::to_string(cx - rx - dx).c_str());
        sValue.append("in,");
        sValue.append(std::to_string(cy - ry - dy).c_str());
        sValue.append("in)");
        propList.insert("draw:transform", sValue);
        painter->drawRectangle(propList);
      }
      else if (style.shape == ELLIPSE) {
        propList.insert("svg:rx", rx);
        propList.insert("svg:ry", ry);
        propList.insert("svg:cx", cx);
        propList.insert("svg:cy", cy);
        propList.insert("librevenge:rotate", -style.rotation);
        painter->drawEllipse(propList);
      }
      else if (style.shape == TRIANGLE) {
        librevenge::RVNGPropertyListVector path;
        librevenge::RVNGPropertyList point;
        switch (style.direction) {
        case NORTH:
          x = cx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        case EAST:
          x = cx - rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        case SOUTH:
          x = cx - rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        case WEST:
          x = cx - rx; y = cy;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        }
        point.insert("librevenge:path-action", "Z");
        path.append(point); point.clear();
        propList.insert("svg:d", path);
        painter->drawPath(propList);
      }
      else if (style.shape == CALLOUT) {
        librevenge::RVNGPropertyListVector path;
        librevenge::RVNGPropertyList point;
        switch (style.direction) {
        case NORTH:
          x = cx - rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx - style.calloutLength/100.; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx - style.calloutLength/100.;
          y = cy + ry*(2*style.calloutPosition - 1) - style.calloutWidth/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry*(2*style.calloutTipPosition - 1);
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx - style.calloutLength/100.;
          y = cy + ry*(2*style.calloutPosition - 1);
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx - style.calloutLength/100.; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        case EAST:
          x = cx - rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry - style.calloutLength/100.;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx*(2*style.calloutPosition - 1) + style.calloutWidth/100;
          y = cy + ry - style.calloutLength/100.;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx*(2*style.calloutPosition - 1); y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx*(2*style.calloutPosition - 1);
          y = cy + ry - style.calloutLength/100.;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry - style.calloutLength/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        case SOUTH:
          x = cx - rx + style.calloutLength/100; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx + style.calloutLength/100; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx + style.calloutLength/100;
          y = cy + ry*(2*style.calloutPosition - 1) + style.calloutWidth/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx + style.calloutLength/100;
          y = cy + ry*(2*style.calloutPosition - 1);
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry*(2*style.calloutPosition - 1);
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        case WEST:
          x = cx - rx; y = cy - ry + style.calloutLength/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx*(2*style.calloutPosition - 1) - style.calloutWidth/100;
          y = cy - ry + style.calloutLength/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx*(2*style.calloutPosition - 1); y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx*(2*style.calloutPosition - 1);
          y = cy - ry + style.calloutLength/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy - ry + style.calloutLength/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        }
        point.insert("librevenge:path-action", "Z");
        path.append(point); point.clear();
        propList.insert("svg:points", path);
        painter->drawPolygon(propList);
      }
      else if (style.shape == PROCESS) {
        librevenge::RVNGPropertyListVector path;
        librevenge::RVNGPropertyList point;
        switch (style.direction) {
        case NORTH:
        case SOUTH:
          x = cx - rx; y = cy + ry*(2*style.processBarSize - 1);
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry*(2*style.processBarSize - 1);
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          point.insert("librevenge:path-action", "Z");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry*(1 - 2*style.processBarSize);
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry*(1 - 2*style.processBarSize);
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          point.insert("librevenge:path-action", "Z");
          path.append(point); point.clear();
          break;
        case EAST:
        case WEST:
          x = cx + rx*(2*style.processBarSize - 1); y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx*(2*style.processBarSize - 1); y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          point.insert("librevenge:path-action", "Z");
          path.append(point); point.clear();
          x = cx + rx*(1 - 2*style.processBarSize); y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx*(1 - 2*style.processBarSize); y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          point.insert("librevenge:path-action", "Z");
          path.append(point); point.clear();
          break;
        }
        x = cx - rx; y = cy - ry;
        point = getPoint(x, y, cx, cy, angle);
        point.insert("librevenge:path-action", "M");
        path.append(point); point.clear();
        x = cx + rx; y = cy - ry;
        point = getPoint(x, y, cx, cy, angle);
        point.insert("librevenge:path-action", "L");
        path.append(point); point.clear();
        x = cx + rx; y = cy + ry;
        point = getPoint(x, y, cx, cy, angle);
        point.insert("librevenge:path-action", "L");
        path.append(point); point.clear();
        x = cx - rx; y = cy + ry;
        point = getPoint(x, y, cx, cy, angle);
        point.insert("librevenge:path-action", "L");
        path.append(point); point.clear();
        point.insert("librevenge:path-action", "Z");
        path.append(point); point.clear();
        propList.insert("svg:d", path);
        painter->drawPath(propList);
      }
      else if (style.shape == RHOMBUS) {
        librevenge::RVNGPropertyListVector path;
        librevenge::RVNGPropertyList point;
        x = cx; y = cy - ry;
        point = getPoint(x, y, cx, cy, angle);
        point.insert("librevenge:path-action", "M");
        path.append(point); point.clear();
        x = cx + rx; y = cy;
        point = getPoint(x, y, cx, cy, angle);
        point.insert("librevenge:path-action", "L");
        path.append(point); point.clear();
        x = cx; y = cy + ry;
        point = getPoint(x, y, cx, cy, angle);
        point.insert("librevenge:path-action", "L");
        path.append(point); point.clear();
        x = cx - rx; y = cy;
        point = getPoint(x, y, cx, cy, angle);
        point.insert("librevenge:path-action", "L");
        path.append(point); point.clear();
        point.insert("librevenge:path-action", "Z");
        path.append(point); point.clear();
        propList.insert("svg:d", path);
        painter->drawPath(propList);
      }
      else if (style.shape == PARALLELOGRAM) {
        librevenge::RVNGPropertyListVector path;
        librevenge::RVNGPropertyList point;
        switch (style.direction) {
        case NORTH:
        case SOUTH:
          x = cx - rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy - ry + style.parallelogramSize/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry - style.parallelogramSize/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        case EAST:
        case WEST:
          x = cx - rx + style.parallelogramSize/100; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx - style.parallelogramSize/100; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        }
        point.insert("librevenge:path-action", "Z");
        path.append(point); point.clear();
        propList.insert("svg:d", path);
        painter->drawPath(propList);
      }
      else if (style.shape == HEXAGON) {
        librevenge::RVNGPropertyListVector path;
        librevenge::RVNGPropertyList point;
        switch (style.direction) {
        case NORTH:
        case SOUTH:
          x = cx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy - ry + style.hexagonSize/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry - style.hexagonSize/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry - style.hexagonSize/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy - ry + style.hexagonSize/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        case EAST:
        case WEST:
          x = cx - rx; y = cy;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx - rx + style.hexagonSize/100; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx - style.hexagonSize/100; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx - style.hexagonSize/100; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx + style.hexagonSize/100; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        }
        point.insert("librevenge:path-action", "Z");
        path.append(point); point.clear();
        propList.insert("svg:d", path);
        painter->drawPath(propList);
      } else if (style.shape == STEP) {
        librevenge::RVNGPropertyListVector path;
        librevenge::RVNGPropertyList point;
        switch (style.direction) {
        case NORTH:
          x = cx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy - ry + style.stepSize/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx; y = cy + ry - style.stepSize/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy - ry + style.stepSize/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        case SOUTH:
          x = cx; y = cy - ry + style.stepSize;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx + rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry - style.stepSize/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry - style.stepSize/100;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        case EAST:
          x = cx - rx + style.stepSize/100; y = cy;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx - rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx - style.stepSize/100; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx - style.stepSize/100; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        case WEST:
          x = cx - rx; y = cy;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "M");
          path.append(point); point.clear();
          x = cx - rx + style.stepSize/100; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy - ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx - style.stepSize/100; y = cy;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx + rx; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          x = cx - rx + style.stepSize/100; y = cy + ry;
          point = getPoint(x, y, cx, cy, angle);
          point.insert("librevenge:path-action", "L");
          path.append(point); point.clear();
          break;
        }
        point.insert("librevenge:path-action", "Z");
        path.append(point); point.clear();
        propList.insert("svg:d", path);
        painter->drawPath(propList);
      }
    }
    if (!data.label.empty()) {
      propList.clear();
      propList.insert("svg:x", geometry.x / 100.);
      propList.insert("svg:y", geometry.y / 100.);
      propList.insert("svg:width", geometry.width / 100.);
      propList.insert("svg:height", geometry.height / 100.);
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

  librevenge::RVNGPropertyList MXCell::getPoint(
    double x, double y, double cx, double cy, double angle
  )
  {
    librevenge::RVNGPropertyList point;
    double r = sqrt(pow(x - cx, 2) + pow(y - cy, 2));
    double old_angle = x != cx ?
      atan((y-cy) / (x-cx)) + (x > cx ? 0 : boost::math::double_constants::pi) :
      (y <= cy ? -1 : 1) * boost::math::double_constants::pi / 2;
    point.insert("svg:x", cx + r*cos(old_angle - angle));
    point.insert("svg:y", cy + r*sin(old_angle - angle));
    return point;
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
    it = style_m.find("entryDx"); if (it != style_m.end())
      style.entryDx = std::stod(it->second);
    it = style_m.find("entryDy"); if (it != style_m.end())
      style.entryDy = std::stod(it->second);
    it = style_m.find("exitDx"); if (it != style_m.end())
      style.exitDx = std::stod(it->second);
    it = style_m.find("exitDy"); if (it != style_m.end())
      style.exitDy = std::stod(it->second);
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
    it = style_m.find("rhombus"); if (it != style_m.end()) style.shape = RHOMBUS;
    it = style_m.find("shape"); if (it != style_m.end()) {
      if (it->second == "callout") style.shape = CALLOUT;
      else if (it->second == "process") style.shape = PROCESS;
      else if (it->second == "parallelogram") style.shape = PARALLELOGRAM;
      else if (it->second == "hexagon") style.shape = HEXAGON;
      else if (it->second == "step") style.shape = STEP;
    }
    style.perimeter = default_perimeter.at(style.shape);
    it = style_m.find("direction"); if (it != style_m.end()) {
      if (it->second == "north") style.direction = NORTH;
      else if (it->second == "east") style.direction = EAST;
      else if (it->second == "south") style.direction = SOUTH;
      else if (it->second == "west") style.direction = WEST;
    }
    it = style_m.find("fixedSize"); if (it != style_m.end()) {
      std::istringstream(it->second) >> style.fixedSize;
    }
    it = style_m.find("size"); if (it != style_m.end()) {
      if (style.shape == CALLOUT) style.calloutLength = std::stod(it->second);
      else if (style.shape == PROCESS) style.processBarSize = std::stod(it->second);
      else if (style.shape == PARALLELOGRAM)
        style.parallelogramSize = std::stod(it->second);
      else if (style.shape == HEXAGON) style.hexagonSize = std::stod(it->second);
      else if (style.shape == STEP) {
        style.stepSize = std::stod(it->second);
        if (!style.fixedSize)
          style.stepSize *=
            (style.direction == NORTH || style.direction == SOUTH
             ? geometry.height : geometry.width);
      }
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
    it = style_m.find("rotation"); if (it != style_m.end())
      style.rotation = xmlStringToDouble((xmlChar*)(it->second.c_str()));
  }

  void MXCell::setEndPoints(std::map<librevenge::RVNGString, MXCell> id_map) {
    // calculates endpoints for an edge
    // necessary because draw.io doesn't store endpoint coordinates
    // if the edge is attached to a vertex.
    if (!edge) return;
    if (!source_id.empty()) {
      MXCell source = id_map[source_id];
      bool fixed = style.exitX.has_value() && style.exitY.has_value();
      if (!fixed) {
        if (!target_id.empty()) {
          MXCell target = id_map.at(target_id);
          style.exitX =
            (source.geometry.x + source.geometry.width < target.geometry.x) ? 1 :
            (source.geometry.x > target.geometry.x + target.geometry.width) ? 0 : 0.5;
          if (style.exitX.get() == 0.5) {
            style.exitY = (source.geometry.y < target.geometry.y) ? 1 : 0;
          } else style.exitY = 0.5;
        } else { // target point specified
          style.exitX =
            (source.geometry.x + source.geometry.width < geometry.targetPoint.x) ? 1 :
            (source.geometry.x > geometry.targetPoint.x) ? 0 : 0.5;
          if (style.exitX.get() == 0.5)
            style.exitY = (source.geometry.y < geometry.targetPoint.y) ? 1 : 0;
          else style.exitY = 0.5;
        }
        geometry.sourcePoint.x =
          source.geometry.x + (style.exitX.get() * source.geometry.width);
        geometry.sourcePoint.y =
          source.geometry.y + (style.exitY.get() * source.geometry.height);
      }
      else {
        double outX = style.exitX.get(); double outY = style.exitY.get();
        adjustEndpoint(&outX, &outY, source);
        double x, y;
        switch (source.style.direction) {
        case EAST:
          x = (source.geometry.x
               + (outX * source.geometry.width)
               + style.exitDx);
          y = (source.geometry.y
               + (outY * source.geometry.height)
               + style.exitDy);
          break;
        case WEST:
          x = (source.geometry.x
               + ((1 - outX) * source.geometry.width)
               - style.exitDx);
          y = (source.geometry.y
               + ((1 - outY) * source.geometry.height)
               - style.exitDy);
          break;
        case NORTH:
          x = (source.geometry.x
               + (outY * source.geometry.width)
               + style.exitDy);
          y = (source.geometry.y
               + ((1 - outX) * source.geometry.height)
               - style.exitDx);
          break;
        case SOUTH:
          x = (source.geometry.x
               + ((1 - outY) * source.geometry.width)
               - style.exitDy);
          y = (source.geometry.y
               + (outX * source.geometry.height)
               + style.exitDx);
          break;
        }
        double cx = source.geometry.x + source.geometry.width / 2;
        double cy = source.geometry.y + source.geometry.height / 2;
        double angle = -source.style.rotation * boost::math::double_constants::pi / 180;
        librevenge::RVNGPropertyList point = getPoint(x, y, cx, cy, angle);
        geometry.sourcePoint.x = point["svg:x"]->getDouble();
        geometry.sourcePoint.y = point["svg:y"]->getDouble();
      }
    }
    if (!target_id.empty()) {
      MXCell target = id_map[target_id];
      bool fixed = style.entryX.has_value() && style.entryY.has_value();
      if (!fixed) {
        if (!source_id.empty()) {
          MXCell source = id_map.at(source_id);
          style.entryY =
            (target.geometry.y > source.geometry.y + source.geometry.height) ? 0 :
            (target.geometry.y + target.geometry.height < source.geometry.y) ? 1 : 0.5;
          if (style.entryY.get() == 0.5)
            style.entryX = (target.geometry.x < source.geometry.x) ? 1 : 0;
          else style.entryX = 0.5;
        } else { // source point specified
          style.entryY =
            (target.geometry.y + target.geometry.height < geometry.sourcePoint.y) ? 1 :
            (target.geometry.y > geometry.sourcePoint.y) ? 0 : 0.5;
          if (style.entryY.get() == 0.5)
            style.entryX = (target.geometry.x < geometry.sourcePoint.x) ? 1 : 0;
          else style.entryX = 0.5;
        }
        geometry.targetPoint.x =
          target.geometry.x + (style.entryX.get() * target.geometry.width);
        geometry.targetPoint.y =
          target.geometry.y + (style.entryY.get() * target.geometry.height);
      } else {
        double outX = style.entryX.get(); double outY = style.entryY.get();
        adjustEndpoint(&outX, &outY, target);
        double x, y;
        switch (target.style.direction) {
        case EAST:
          x = (target.geometry.x
               + (outX * target.geometry.width)
               + style.entryDx);
          y = (target.geometry.y
               + (outY * target.geometry.height)
               + style.entryDy);
          break;
        case WEST:
          x = (target.geometry.x
               + ((1 - outX) * target.geometry.width)
               - style.entryDx);
          y = (target.geometry.y
               + ((1 - outY) * target.geometry.height)
               - style.entryDy);
          break;
        case NORTH:
          x = (target.geometry.x
               + (outY * target.geometry.width)
               + style.entryDy);
          y = (target.geometry.y
               + ((1 - outX) * target.geometry.height)
               - style.entryDx);
          break;
        case SOUTH:
          x = (target.geometry.x
               + ((1 - outY) * target.geometry.width)
               - style.entryDy);
          y = (target.geometry.y
               + (outX * target.geometry.height)
               + style.entryDx);
          break;
        }
        double cx = target.geometry.x + target.geometry.width / 2;
        double cy = target.geometry.y + target.geometry.height / 2;
        double angle = -target.style.rotation * boost::math::double_constants::pi / 180;
        librevenge::RVNGPropertyList point = getPoint(x, y, cx, cy, angle);
        geometry.targetPoint.x = point["svg:x"]->getDouble();
        geometry.targetPoint.y = point["svg:y"]->getDouble();
      }
    }
  }

  void MXCell::adjustEndpoint(double* outX, double* outY, MXCell shape)
  {
    double x = *outX; double y = *outY;
    bool on_edge = x == 0 || x == 1 || y == 0 || y == 1;
    if (on_edge) {
      if (shape.style.perimeter == TRIANGLE_P) {
        // update exitX, exitY to connect with diagonal edges of triangle
        if (y < 0.5 && x > 0) {
          double m = (x - 0.5) / (y - 0.5);
          *outY = (0.5 - m/2) / (2 - m);
          *outX = 2 * *outY;
        } else if (y > 0.5 && x > 0) {
          double m = (x - 0.5) / (y - 0.5);
          *outY = (1.5 + m/2) / (2 + m);
          *outX = 2 - 2 * *outY;
        }
      } else if (shape.style.perimeter == ELLIPSE_P) {
        if (x != 0.5) {
          double m = (y - 0.5) / (x - 0.5);
          double t = atan(m) + (x < 0.5 ? boost::math::double_constants::pi : 0);
          *outX = 0.5 + 0.5*cos(t);
          *outY = 0.5 + 0.5*sin(t);
        }
      } else if (shape.style.perimeter == RHOMBUS_P) {
        if (x < 0.5 && y < 0.5) {
          double m = (y - 0.5) / (x - 0.5);
          *outX = m / (2 + 2*m);
          *outY = -*outX + 0.5;
        } else if (x < 0.5 && y > 0.5) {
          double m = (y - 0.5) / (x - 0.5);
          *outX = m / (2*m - 2);
          *outY = *outX + 0.5;
        } else if (x > 0.5 && y < 0.5) {
          double m = (y - 0.5) / (x - 0.5);
          *outX = (m - 2) / (2*m - 2);
          *outY = *outX - 0.5;
        } else if (x > 0.5 && y > 0.5) {
          double m = (y - 0.5) / (x - 0.5);
          *outX = (m + 2) / (2*m + 2);
          *outY = -*outX + 1.5;
        }
      } else if (shape.style.perimeter == PARALLELOGRAM_P) {
        double c =
          (shape.style.parallelogramSize
           / (shape.style.direction == NORTH || shape.style.direction == SOUTH
              ? shape.geometry.height : shape.geometry.width));
        c = std::min(c, 0.5);
        if (c != 0 && x != 0.5) {
          double m = (y - 0.5) / (x - 0.5);
          if (x < c && y < 1) {
            *outX = (m*c + c) / (2*m*c + 2);
            *outY = 1 - *outX/c;
          } else if (x > 1 - c && y > 0) {
            *outX = (m*c - c + 2) / (2*m*c + 2);
            *outY = (1 - *outX) / c;
          }
        }
      } else if (shape.style.perimeter == HEXAGON_P) {
        double c =
          (shape.style.hexagonSize
           / (shape.style.direction == NORTH || shape.style.direction == SOUTH
              ? shape.geometry.height : shape.geometry.width));
        c = std::min(c, 0.5);
        if (c != 0 && x != 0.5) {
          double m = (y - 0.5) / (x - 0.5);
          if (x < c && y < 0.5) {
            *outX = c * m / (2*c*m + 1);
            *outY = -*outX/(2*c) + 0.5;
          } else if (x > 1 - c && y < 0.5) {
            *outX = (m*c - 1) / (2*m*c - 1);
            *outY = (*outX + c - 1) / (2*c);
          } else if (x < c && y > 0.5) {
            *outX = m*c / (2*m*c - 1);
            *outY = *outX/(2*c) + 0.5;
          } else if (x > 1 - c && y > 0.5) {
            *outX = (m*c + 1) / (2*m*c + 1);
            *outY = (1 + c - *outX) / (2*c);
          }
        }
      } else if (shape.style.perimeter == STEP_P) {
        double c =
          (shape.style.stepSize
           / (shape.style.direction == NORTH || shape.style.direction == SOUTH
              ? shape.geometry.height : shape.geometry.width));
        if (x == 0 && c > 0.5 && 0 < y && y < 1) {
          *outX = 0.5; *outY = 0.5;
        } else if (y == 0.5) {
          *outX = x == 0 ? c : 1;
        } else {
          double m = (x - 0.5) / (y - 0.5);
          if (x == 0 && y < 0.5) {
            *outY = (1 - m) / (4*c - 2*m);
            *outX = 2*c**outY;
          } else if (x == 0 && y > 0.5) {
            *outY = (4*c - 1 + m) / (4*c + 2*m);
            *outX = 2*c - 2*c**outY;
          } else if (x > 1 - c && y < 0.5) {
            *outY = (1 - 2*c + m) / (2*m - 4*c);
            *outX = 2*c**outY + 1 - c;
          } else if (x > 1 - c && y > 0.5) {
            *outY = (1 + 2*c + m) / (4*c + 2*m);
            *outX = 1 + c - 2*c**outY;
          }
        }
      }
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
