/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef MXCELL_H
#define MXCELL_H

#include "DRAWIOStyle.h"
#include "DRAWIOTypes.h"
#include "MXGeometry.h"
#include "DRAWIOUserObject.h"
#include "librevenge/RVNGPropertyList.h"
#include "librevenge/RVNGString.h"
#include "librevenge/librevenge.h"
#include <map>
#include <string>
#include <vector>

namespace libdrawio {
  struct MXCell {
    librevenge::RVNGString id;
    DRAWIOUserObject data;
    MXGeometry geometry;
    librevenge::RVNGString style_str;
    void setStyle();
    DRAWIOStyle style;
    bool vertex, edge, connectable, visible, collapsed;
    librevenge::RVNGString parent_id, source_id, target_id;
    std::vector<librevenge::RVNGString> children; // holds references to child cells
    std::vector<librevenge::RVNGString> edges; // holds references to connected edges
    MXCell()
      : id(), data(), geometry(), style(), vertex(), edge(), connectable(),
        visible(), collapsed(), parent_id(), source_id(), target_id(), children(),
        edges() {}
    MXCell(const MXCell &mxcell) = default;
    MXCell &operator=(const MXCell &mxcell) = default;
    void draw(librevenge::RVNGDrawingInterface *painter,
              std::map<librevenge::RVNGString, MXCell> id_map);
    void setEndPoints(std::map<librevenge::RVNGString, MXCell> id_map);
    librevenge::RVNGPropertyList getStyle();
  private:
    struct Bounds {
      int x, y;
      int width, height;
    };
    void calculateBounds();
    Bounds bounds;
    std::string getViewBox();
    librevenge::RVNGPropertyListVector getPath();
    librevenge::RVNGString processText(librevenge::RVNGString input);
    std::string getMarkerViewBox(MarkerType marker);
    std::string getMarkerPath(MarkerType marker);
    librevenge::RVNGPropertyList getPoint(
      double x, double y, double cx, double cy, double angle
    );
    void adjustEndpoint(double& outX, double& outY, const MXCell& shape);
    void setEndpointInShape(double x, double y, const MXCell& shape, MXPoint& point,
                            double dx = 0, double dy = 0);
    void setWaypoints(std::map<librevenge::RVNGString, MXCell> id_map);
    static int draw_count;
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
