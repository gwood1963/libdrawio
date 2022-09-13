/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef DRAWIOTYPES_H
#define DRAWIOTYPES_H

namespace libdrawio {
  enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
  };

  enum AlignH {
    LEFT,
    CENTER,
    RIGHT
  };

  enum AlignV {
    TOP,
    MIDDLE,
    BOTTOM
  };

  struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    Color(unsigned char red, unsigned char green,
          unsigned char blue, unsigned char alpha)
      : r(red), g(green), b(blue), a(alpha) {}
    Color() : r(0), g(0), b(0), a(0) {}
    inline bool operator==(const Color &col) {
      return ((r == col.r) && (g == col.g) && (b == col.b) && (a == col.a));
    }
    inline bool operator!=(const Color &col) {
      return !operator==(col);
    }
    inline bool operator!() const {
      return (!r && !g && !b && !a);
    }
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
