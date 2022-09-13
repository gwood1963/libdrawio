/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef DRAWIOUSEROBJECT_H
#define DRAWIOUSEROBJECT_H

#include <string>
#include <map>

namespace libdrawio {
  struct DRAWIOUserObject {
    std::string label;
    std::map<std::string, std::string> data;
    DRAWIOUserObject() : label(), data() {}
    DRAWIOUserObject(const DRAWIOUserObject &obj) = default;
    DRAWIOUserObject &operator=(const DRAWIOUserObject &obj) = default;
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
