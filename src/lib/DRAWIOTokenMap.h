/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __DRAWIOTOKENMAP_H__
#define __DRAWIOTOKENMAP_H__

#include <libxml/xmlstring.h>
#include "tokens.h"

namespace libdrawio {
  class DRAWIOTokenMap {
  public:
    static int getTokenId(const xmlChar *name);
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
