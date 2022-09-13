/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef DRAWIOSTYLE_H
#define DRAWIOSTYLE_H

#include <boost/optional.hpp>

namespace libdrawio {
  struct DRAWIOStyle {
    double opacity; // opacity in [0,100]
    bool orthogonal;
    double entryX, entryY; // in [0,1]
    double exitX, exitY; // same range as above
    bool wrapText;
    double rotation; // rotation in [0,360]
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
