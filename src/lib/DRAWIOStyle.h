/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef DRAWIOSTYLE_H
#define DRAWIOSTYLE_H

#include <boost/optional.hpp>
#include <boost/optional/optional.hpp>
#include "DRAWIOTypes.h"

namespace libdrawio {
  struct DRAWIOStyle {
    double opacity; // opacity in [0,100]
    bool orthogonal;
    double entryX, entryY; // in [0,1]
    double exitX, exitY; // same range as above
    bool wrapText;
    double rotation; // rotation in [0,360]
    boost::optional<Color> fillColor;
    double margin;
    Color gradientColor;
    Direction gradientDirection;
    boost::optional<Color> strokeColor;
    double strokeWidth;
    AlignH align;
    AlignV verticalAlign;
    AlignH labelPosition;
    AlignV verticalLabelPosition;
    bool flipH, flipV;
    boost::optional<Color> labelBackgroundColor;
    boost::optional<Color> labelBorderColor;
    bool shadow;
    bool dashed;
    bool rounded;
    bool curved;
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
