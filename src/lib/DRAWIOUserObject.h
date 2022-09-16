/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef DRAWIOUSEROBJECT_H
#define DRAWIOUSEROBJECT_H

#include "librevenge/RVNGString.h"
#include <string>
#include <map>

namespace libdrawio {
  struct DRAWIOUserObject {
    librevenge::RVNGString label;
    std::map<librevenge::RVNGString, librevenge::RVNGString> data;
    DRAWIOUserObject() : label(), data() {}
    DRAWIOUserObject(librevenge::RVNGString label) : label(label), data() {}
    DRAWIOUserObject(const DRAWIOUserObject &obj) = default;
    DRAWIOUserObject &operator=(const DRAWIOUserObject &obj) = default;
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
