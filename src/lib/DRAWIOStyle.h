/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef DRAWIOSTYLE_H
#define DRAWIOSTYLE_H

#include <boost/optional.hpp>
#include "DRAWIOTypes.h"

namespace libdrawio {
  struct DRAWIOStyle {
    double opacity; // opacity in [0,100]
    bool orthogonal;
    boost::optional<double> entryX, entryY; // in [0,1]
    boost::optional<double> exitX, exitY; // same range as above
    double entryDx, entryDy, exitDx, exitDy;
    bool wrapText;
    double rotation; // rotation in [0,360]
    boost::optional<Color> fillColor = Color(255, 255, 255, 1);
    double margin;
    boost::optional<Color> gradientColor;
    Direction gradientDirection;
    boost::optional<Color> strokeColor = Color(0, 0, 0, 1);
    double strokeWidth;
    AlignH align = CENTER;
    AlignV verticalAlign = MIDDLE;
    AlignH labelPosition = CENTER;
    AlignV verticalLabelPosition = MIDDLE;
    bool flipH, flipV;
    boost::optional<Color> labelBackgroundColor;
    boost::optional<Color> labelBorderColor;
    bool shadow;
    bool dashed;
    bool rounded;
    bool curved;
    boost::optional<Direction> portConstraint;
    boost::optional<Direction> sourcePortConstraint;
    boost::optional<Direction> targetPortConstraint;
    Shape shape = RECTANGLE;
    Perimeter perimeter = RECTANGLE_P;
    Direction direction = EAST;
    double calloutLength = 30;
    double calloutWidth = 20;
    double calloutPosition = 0.5;
    double calloutTipPosition = 0.5;
    double processBarSize = 0.1;
    double parallelogramSize = 20;
    double hexagonSize = 20;
    double stepSize = 20;
    double trapezoidSize = 20;
    bool fixedSize = true;
    boost::optional<MarkerType> startArrow;
    bool startFill = true;
    double startSize = 6;
    boost::optional<MarkerType> endArrow = CLASSIC;
    bool endFill = true;
    double endSize = 6;
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
