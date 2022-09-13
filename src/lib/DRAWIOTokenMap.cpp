/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "DRAWIOTokenMap.h"
#include <string.h>

namespace {
#include "tokenhash.h"
}

int libdrawio::DRAWIOTokenMap::getTokenId(const xmlChar *name) {
  const xmltoken *token = Perfect_Hash::in_word_set((const char *)name,
						    xmlStrlen(name));
  if (token) {
    return token->tokenId;
  } else {
    return XML_TOKEN_INVALID;
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
