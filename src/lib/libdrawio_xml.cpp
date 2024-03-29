/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "libdrawio_xml.h"
#include "libdrawio_utils.h"
#include "librevenge-stream/RVNGStream.h"
#include "libxml/xmlreader.h"
#include <libxml/xmlstring.h>
#include <cstring>
#include <libxml/parser.h>
#include <memory>
#include "DRAWIOTypes.h"
#include <boost/lexical_cast.hpp>
#include <string>

namespace {
  extern "C" {
    static int drawioInputCloseFunc(void *) {
      return 0;
    }

    static int drawioInputReadFunc(void *context, char *buffer, int len) {
      auto *input = (librevenge::RVNGInputStream *)context;

      if ((!input) || (!buffer) || (len < 0))
        return -1;

      if (input->isEnd())
        return 0;

      unsigned long tmpNumBytesRead = 0;
      const unsigned char *tmpBuffer = input->read(len, tmpNumBytesRead);

      if (tmpBuffer && tmpNumBytesRead)
        memcpy(buffer, tmpBuffer, tmpNumBytesRead);
      return tmpNumBytesRead;
    }
    
#ifdef DEBUG
    static void drawioReaderErrorFunc(void *arg, const char *message,
				      xmlParserSeverities severity,
				      xmlTextReaderLocatorPtr)
#else
    static void drawioReaderErrorFunc(void *arg, const char *,
				      xmlParserSeverities severity,
				      xmlTextReaderLocatorPtr)
#endif
    {
      auto *const watcher = reinterpret_cast<libdrawio::XMLErrorWatcher *>(arg);
      switch (severity) {
      case XML_PARSER_SEVERITY_VALIDITY_WARNING:
	DRAWIO_DEBUG_MSG(("Found xml parser severity validity warning %s\n",
			  message));
	break;
      case XML_PARSER_SEVERITY_VALIDITY_ERROR:
	DRAWIO_DEBUG_MSG(("Found xml parser severity validity error %s\n",
			  message));
	break;
      case XML_PARSER_SEVERITY_WARNING:
	DRAWIO_DEBUG_MSG(("Found xml parser severity warning %s\n", message));
	break;
      case XML_PARSER_SEVERITY_ERROR:
	DRAWIO_DEBUG_MSG(("Found xml parser severity error %s\n", message));
	if (watcher)
	  watcher->setError();
	break;
      default:
	break;
      }
    }
  }
}

namespace libdrawio {
  XMLErrorWatcher::XMLErrorWatcher() : m_error(false) {
  }

  bool XMLErrorWatcher::isError() const {
    return m_error;
  }
  
  void XMLErrorWatcher::setError() {
    m_error = true;
  }
  
  std::unique_ptr<xmlTextReader, void (*)(xmlTextReaderPtr)>
  xmlReaderForStream(librevenge::RVNGInputStream *input,
                     XMLErrorWatcher *const watcher, bool recover) {
    int options = XML_PARSE_NOBLANKS | XML_PARSE_NONET;
    if (recover)
      options |= XML_PARSE_RECOVER;
    std::unique_ptr<xmlTextReader, void (*)(xmlTextReaderPtr)> reader {
      xmlReaderForIO(drawioInputReadFunc, drawioInputCloseFunc, (void *)input,
		     nullptr, nullptr, options),
      xmlFreeTextReader
    };
    if (reader)
      xmlTextReaderSetErrorHandler(reader.get(), drawioReaderErrorFunc,
				   watcher);
    return reader;
  }

  Color xmlStringToColor(const xmlChar *s) {
    std::string str((const char *)s);
    if (str[0] == '#') {
      if (str.length() != 7) {
        DRAWIO_DEBUG_MSG((("Throwing XmlParserException from XmlStringToColor, color=" + str + "\n").c_str()));
        throw XmlParserException();
      } else
        str.erase(str.begin());
    } else {
      if (str.length() != 6) {
        DRAWIO_DEBUG_MSG((("Throwing XmlParserException from XmlStringToColor, color=" + str + "\n").c_str()));
        throw XmlParserException();
      }
    }

    std::istringstream istr(str);
    unsigned val = 0;
    istr >> std::hex >> val;

    return Color((val & 0xff0000) >> 16, (val & 0xff00) >> 8, val & 0xff, 0);
  }

  Color xmlStringToColor(const std::shared_ptr<xmlChar> &s) {
    return xmlStringToColor(s.get());
  }

  long xmlStringToLong(const xmlChar *s) {
    using boost::lexical_cast;
    using boost::bad_lexical_cast;

    try {
      return boost::lexical_cast<long, const char *>((const char *)s);
    } catch (const boost::bad_lexical_cast &) {
      DRAWIO_DEBUG_MSG(("Throwing XmlParserException from XmlStringToLong\n"));
      throw XmlParserException();
    }
    return 0;
  }

  long xmlStringToLong(const std::shared_ptr<xmlChar> &s) {
    return xmlStringToLong(s.get());
  }

  double xmlStringToDouble(const xmlChar *s) try {
    return boost::lexical_cast<double, const char *>((const char *)s);
  } catch (const boost::bad_lexical_cast &) {
    DRAWIO_DEBUG_MSG(("Throwing XmlParserException from xmlStringToDouble\n"));
    throw XmlParserException();
  }

  double xmlStringToDouble(const std::shared_ptr<xmlChar> &s) {
    return xmlStringToDouble(s.get());
  }

  bool xmlStringToBool(const xmlChar *s) {
    bool value = false;
    if (xmlStrEqual(s, BAD_CAST("true")) || xmlStrEqual(s, BAD_CAST("1")))
      value = true;
    else if (xmlStrEqual(s, BAD_CAST("false")) || xmlStrEqual(s, BAD_CAST("0")))
      value = false;
    else {
      DRAWIO_DEBUG_MSG(("Throwing XmlParserException from xmlStringToBool\n"));
      throw XmlParserException();
    }
    return value;
  }
  
  bool xmlStringToBool(const std::shared_ptr<xmlChar> &s) {
    return xmlStringToBool(s.get());
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
