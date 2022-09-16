/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef LIBDRAWIO_XML_H
#define LIBDRAWIO_XML_H

#include "DRAWIOTypes.h"
#include "librevenge-stream/RVNGStream.h"
#include "libxml/xmlstring.h"
#include <libxml/xmlreader.h>
#include <memory>

namespace libdrawio {
  class XMLErrorWatcher {
    XMLErrorWatcher(const XMLErrorWatcher &);
    XMLErrorWatcher &operator=(const XMLErrorWatcher &);
  public:
    XMLErrorWatcher();
    bool isError() const;
    void setError();
  private:
    bool m_error;
  };

  struct Color;
  
  std::unique_ptr<xmlTextReader, void (*)(xmlTextReaderPtr)>
  xmlReaderForStream(librevenge::RVNGInputStream *input,
                     XMLErrorWatcher *watcher = nullptr, bool recover = true);

  Color xmlStringToColor(const xmlChar *s);
  Color xmlStringToColor(const std::shared_ptr<xmlChar> &s);

  long xmlStringToLong(const xmlChar *s);
  long xmlStringToLong(const std::shared_ptr<xmlChar> &s);

  double xmlStringToDouble(const xmlChar *s);
  double xmlStringToDouble(const std::shared_ptr<xmlChar> &s);

  bool xmlStringToBool(const xmlChar *s);
  bool xmlStringToBool(const std::shared_ptr<xmlChar> &s);
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
