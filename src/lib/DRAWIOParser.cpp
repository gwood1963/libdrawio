/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "DRAWIOParser.h"
#include "DRAWIOPage.h"
#include "DRAWIOTypes.h"
#include "DRAWIOUserObject.h"
#include "MXCell.h"
#include "MXGeometry.h"
#include "libdrawio_utils.h"
#include "librevenge-stream/librevenge-stream.h"
#include "librevenge/RVNGPropertyList.h"
#include "librevenge/librevenge.h"
#include "libdrawio_xml.h"
#include "libxml/globals.h"
#include "libxml/xmlreader.h"
#include "DRAWIOTokenMap.h"
#include "libxml/xmlstring.h"
#include "tokens.h"
#include <string.h>
#include <memory>
#include <vector>

using std::shared_ptr;

namespace libdrawio {
  DRAWIOParser::DRAWIOParser(librevenge::RVNGInputStream *input,
			     librevenge::RVNGDrawingInterface *painter)
    : m_input(input), m_painter(painter), m_value(), m_cell(), m_geometry(),
      m_point(), m_current_page() {}

  DRAWIOParser::~DRAWIOParser() {}

  bool DRAWIOParser::parseMain() {
    if (!m_input)
      return false;
    try {
      m_input->seek(0, librevenge::RVNG_SEEK_SET);
      if (!_processXmlDocument(m_input))
	return false;
      return true;
    } catch (...) {
      return false;
    }
  }

  bool DRAWIOParser::_processXmlDocument(librevenge::RVNGInputStream *input) {
    if (!input)
      return false;

    auto reader = xmlReaderForStream(input);
    if (!reader)
      return false;
    int ret = xmlTextReaderRead(reader.get());
    while (ret == 1) {
      _processXmlNode(reader.get());
      ret = xmlTextReaderRead(reader.get());
    }
    return true;
  }

  void DRAWIOParser::_processXmlNode(xmlTextReaderPtr reader) {
    if (!reader)
      return;
    int tokenId = _getElementToken(reader);
    int tokenType = xmlTextReaderNodeType(reader);
    _handleLevelChange((unsigned)_getElementDepth(reader));
    switch (tokenId) {
    case XML_OBJECT:
      if (tokenType == XML_READER_TYPE_ELEMENT)
        _readObject(reader);
      break;
    case XML_DIAGRAM:
      if (tokenType == XML_READER_TYPE_ELEMENT)
        _startPage(reader);
      if (tokenType == XML_READER_TYPE_END_ELEMENT)
        _endPage();
      break;
    case XML_MXGRAPHMODEL:
      if (tokenType == XML_READER_TYPE_ELEMENT)
        _readGraphModel(reader);
      break;
    case XML_MXCELL:
      if (tokenType == XML_READER_TYPE_ELEMENT)
        _readCell(reader);
      if (tokenType == XML_READER_TYPE_END_ELEMENT)
        _flushCell();
      break;
    case XML_MXGEOMETRY:
      if (tokenType == XML_READER_TYPE_ELEMENT)
        _readGeometry(reader);
      if (tokenType == XML_READER_TYPE_END_ELEMENT
          || xmlTextReaderIsEmptyElement(reader))
        _flushGeometry();
      break;
    case XML_MXPOINT:
      if (tokenType == XML_READER_TYPE_ELEMENT)
        _readPoint(reader);
      break;
    case XML_ARRAY:
      if (tokenType == XML_READER_TYPE_ELEMENT)
        m_in_points_list = true;
      if (tokenType == XML_READER_TYPE_END_ELEMENT)
        m_in_points_list = false;
      break;
    case XML_MXFILE:
      if (tokenType == XML_READER_TYPE_END_ELEMENT)
        _endDocument();
      break;
    default:
      break;
    }
    
  }

  void DRAWIOParser::_readPoint(xmlTextReaderPtr reader) {
    m_point = MXPoint();
    
    const shared_ptr<xmlChar> x(xmlTextReaderGetAttribute(reader, BAD_CAST("x")), xmlFree);
    const shared_ptr<xmlChar> y(xmlTextReaderGetAttribute(reader, BAD_CAST("y")), xmlFree);
    const shared_ptr<xmlChar> as(xmlTextReaderGetAttribute(reader, BAD_CAST("as")), xmlFree);

    m_point.x = xmlStringToDouble(x);
    m_point.y = xmlStringToDouble(y);
    if (as && xmlStrEqual(as.get(), BAD_CAST("sourcePoint")) && m_geometryStarted)
      m_geometry.sourcePoint = m_point;
    else if (as && xmlStrEqual(as.get(), BAD_CAST("targetPoint")) && m_geometryStarted)
      m_geometry.targetPoint = m_point;
    else if (m_in_points_list)
      m_geometry.points.push_back(m_point);
  }

  void DRAWIOParser::_readGeometry(xmlTextReaderPtr reader) {
    m_geometry = MXGeometry();
    m_geometryStarted = true;
    const shared_ptr<xmlChar> x(xmlTextReaderGetAttribute(reader, BAD_CAST("x")), xmlFree);
    const shared_ptr<xmlChar> y(xmlTextReaderGetAttribute(reader, BAD_CAST("y")), xmlFree);
    const shared_ptr<xmlChar> width(xmlTextReaderGetAttribute(reader, BAD_CAST("width")), xmlFree);
    const shared_ptr<xmlChar> height(xmlTextReaderGetAttribute(reader, BAD_CAST("height")), xmlFree);
    const shared_ptr<xmlChar> offset(xmlTextReaderGetAttribute(reader, BAD_CAST("offset")), xmlFree);
    const shared_ptr<xmlChar> relative(xmlTextReaderGetAttribute(reader, BAD_CAST("relative")), xmlFree);

    if (x)
      m_geometry.x = xmlStringToDouble(x.get());
    if (y)
      m_geometry.y = xmlStringToDouble(y.get());
    if (width)
      m_geometry.width = xmlStringToDouble(width.get());
    if (height)
      m_geometry.height = xmlStringToDouble(height.get());
    if (offset)
      m_geometry.offset = xmlStringToDouble(offset.get());
    if (relative)
      m_geometry.relative = xmlStringToBool(relative.get());
  }

  void DRAWIOParser::_flushGeometry() {
    if (m_cellStarted)
      m_cell.geometry = m_geometry;
    m_geometryStarted = false;
  }

  void DRAWIOParser::_readObject(xmlTextReaderPtr reader) {
    m_objectStarted = true;
    m_value = DRAWIOUserObject();

    const shared_ptr<xmlChar>
      label(xmlTextReaderGetAttribute(reader, BAD_CAST("label")), xmlFree);

    if (label) {
      DRAWIOName labelName =
        DRAWIOName(librevenge::RVNGBinaryData(label.get(), xmlStrlen(label.get())),
                   DRAWIO_TEXT_UTF8);
      _convertDataToString(m_value.label, labelName.m_data, labelName.m_format);
    }
  }

  void DRAWIOParser::_readCell(xmlTextReaderPtr reader) {
    m_cell = MXCell();
    m_cellStarted = true;

    const shared_ptr<xmlChar>
      id(xmlTextReaderGetAttribute(reader, BAD_CAST("id")), xmlFree);
    const shared_ptr<xmlChar>
      style(xmlTextReaderGetAttribute(reader, BAD_CAST("style")), xmlFree);
    const shared_ptr<xmlChar>
      source(xmlTextReaderGetAttribute(reader, BAD_CAST("source")), xmlFree);
    const shared_ptr<xmlChar>
      target(xmlTextReaderGetAttribute(reader, BAD_CAST("target")), xmlFree);
    const shared_ptr<xmlChar>
      parent(xmlTextReaderGetAttribute(reader, BAD_CAST("parent")), xmlFree);
    const shared_ptr<xmlChar>
      vertex(xmlTextReaderGetAttribute(reader, BAD_CAST("vertex")), xmlFree);
    const shared_ptr<xmlChar>
      edge(xmlTextReaderGetAttribute(reader, BAD_CAST("edge")), xmlFree);
    const shared_ptr<xmlChar>
      connectable(xmlTextReaderGetAttribute(reader, BAD_CAST("connectable")), xmlFree);
    const shared_ptr<xmlChar>
      visible(xmlTextReaderGetAttribute(reader, BAD_CAST("visible")), xmlFree);
    const shared_ptr<xmlChar>
      collapsed(xmlTextReaderGetAttribute(reader, BAD_CAST("collapsed")), xmlFree);
    const shared_ptr<xmlChar>
      value(xmlTextReaderGetAttribute(reader, BAD_CAST("value")), xmlFree);

    if (id) {
      DRAWIOName idName =
        DRAWIOName(librevenge::RVNGBinaryData(id.get(), xmlStrlen(id.get())),
                   DRAWIO_TEXT_UTF8);
      _convertDataToString(m_cell.id, idName.m_data, idName.m_format);
    }
    if (m_objectStarted) {
      m_cell.data = m_value;
      m_objectStarted = false;
    } else if (value) {
      DRAWIOName valueName =
        DRAWIOName(librevenge::RVNGBinaryData(value.get(), xmlStrlen(value.get())),
                   DRAWIO_TEXT_UTF8);
      _convertDataToString(m_cell.data.label, valueName.m_data, valueName.m_format);
    }
    if (style) {
      DRAWIOName styleName =
        DRAWIOName(librevenge::RVNGBinaryData(style.get(), xmlStrlen(style.get())),
                   DRAWIO_TEXT_UTF8);
      _convertDataToString(m_cell.style, styleName.m_data, styleName.m_format);
    }
    if (source) {
      DRAWIOName sourceName =
        DRAWIOName(librevenge::RVNGBinaryData(source.get(), xmlStrlen(source.get())),
                   DRAWIO_TEXT_UTF8);
      _convertDataToString(m_cell.source, sourceName.m_data, sourceName.m_format);
    }
    if (target) {
      DRAWIOName targetName =
        DRAWIOName(librevenge::RVNGBinaryData(target.get(), xmlStrlen(target.get())),
                   DRAWIO_TEXT_UTF8);
      _convertDataToString(m_cell.target, targetName.m_data, targetName.m_format);
    }
    if (parent) {
      DRAWIOName parentName =
        DRAWIOName(librevenge::RVNGBinaryData(parent.get(), xmlStrlen(parent.get())),
                   DRAWIO_TEXT_UTF8);
      _convertDataToString(m_cell.parent, parentName.m_data, parentName.m_format);
    }
    if (edge)
      m_cell.edge = xmlStringToBool(edge.get());
    if (vertex)
      m_cell.vertex = xmlStringToBool(vertex.get());
    if (collapsed)
      m_cell.collapsed = xmlStringToBool(collapsed.get());
    if (connectable)
      m_cell.connectable = xmlStringToBool(connectable.get());
    if (visible)
      m_cell.visible = xmlStringToBool(visible.get());
  }

  void DRAWIOParser::_flushCell() {
    m_current_page.insert(m_cell);
    m_cellStarted = false;
  }

  int DRAWIOParser::_getElementToken(xmlTextReaderPtr reader) {
    return DRAWIOTokenMap::getTokenId(xmlTextReaderConstName(reader));
  }

  int DRAWIOParser::_getElementDepth(xmlTextReaderPtr reader) {
    return xmlTextReaderDepth(reader);
  }

  void DRAWIOParser::_handleLevelChange(unsigned level) {
    m_current_level = level;
  }

  void DRAWIOParser::_startPage(xmlTextReaderPtr reader) {
    m_current_page = DRAWIOPage();

    const shared_ptr<xmlChar>
      id(xmlTextReaderGetAttribute(reader, BAD_CAST("id")), xmlFree);
    const shared_ptr<xmlChar>
      name(xmlTextReaderGetAttribute(reader, BAD_CAST("name")), xmlFree);

    if (id) {
      DRAWIOName idName =
        DRAWIOName(librevenge::RVNGBinaryData(id.get(), xmlStrlen(id.get())),
                   DRAWIO_TEXT_UTF8);
      _convertDataToString(m_current_page.id, idName.m_data, idName.m_format);
    }
    if (name) {
      DRAWIOName nameName =
        DRAWIOName(librevenge::RVNGBinaryData(name.get(), xmlStrlen(name.get())),
                   DRAWIO_TEXT_UTF8);
      _convertDataToString(m_current_page.name, nameName.m_data, nameName.m_format);
    }
  }

  void DRAWIOParser::_readGraphModel(xmlTextReaderPtr reader) {
    const shared_ptr<xmlChar>
      width(xmlTextReaderGetAttribute(reader, BAD_CAST("pageWidth")), xmlFree);
    const shared_ptr<xmlChar>
      height(xmlTextReaderGetAttribute(reader, BAD_CAST("pageHeight")), xmlFree);

    m_current_page.width = xmlStringToDouble(width.get());
    m_current_page.height = xmlStringToDouble(height.get());
  }

  void DRAWIOParser::_endPage() {
    m_pages.push_back(m_current_page);
  }

  void DRAWIOParser::_endDocument() {
    m_painter -> startDocument(librevenge::RVNGPropertyList());
    for (auto page : m_pages) {
      page.draw(m_painter);
    }
    m_painter->endDocument();
  }

  xmlChar *DRAWIOParser::_readStringData(xmlTextReaderPtr reader) {
    int ret = xmlTextReaderRead(reader);
    if (ret == 1 && xmlTextReaderNodeType(reader) == XML_READER_TYPE_TEXT) {
      std::unique_ptr<xmlChar, void (*)(void *)>
        stringValue(xmlTextReaderValue(reader), xmlFree);
      ret = xmlTextReaderRead(reader);
      if (ret == 1 && stringValue) {
        DRAWIO_DEBUG_MSG(("DRAWIOParser::readStringData stringValue %s\n",
                          (const char *)stringValue.get()));
        return stringValue.release();
      }
    }
    return nullptr;
  }

  int DRAWIOParser::_readStringData(DRAWIOName &text, xmlTextReaderPtr reader) {
    const shared_ptr<xmlChar> stringValue(_readStringData(reader), xmlFree);
    if (stringValue) {
      DRAWIO_DEBUG_MSG(("DRAWIOParser::_readStringData stringValue %s\n",
                        (const char *)stringValue.get()));
      text.m_data = librevenge::RVNGBinaryData(stringValue.get(),
                                               xmlStrlen(stringValue.get()));
      text.m_format = DRAWIO_TEXT_UTF8;
      return 1;
    }
    return -1;
  }

  int DRAWIOParser::_readBoolData(bool &value, xmlTextReaderPtr reader) {
    const shared_ptr<xmlChar> stringValue(_readStringData(reader), xmlFree);
    if (stringValue) {
      DRAWIO_DEBUG_MSG(("DRAWIOParser::_readBoolData stringValue %s\n",
                        (const char *)stringValue.get()));
      value = xmlStringToBool(stringValue); 
      return 1;
    }
    return -1;
  }

  int DRAWIOParser::_readDoubleData(double &value, xmlTextReaderPtr reader) {
    const shared_ptr<xmlChar> stringValue(_readStringData(reader), xmlFree);
    if (stringValue) {
      DRAWIO_DEBUG_MSG(("DRAWIOParser::_readDoubleData stringValue %s\n",
                        (const char *)stringValue.get()));
      value = xmlStringToDouble(stringValue); 
      return 1;
    }
    return -1;
  }

  void DRAWIOParser::_convertDataToString(librevenge::RVNGString &result,
                                          const librevenge::RVNGBinaryData &data,
                                          TextFormat format) {
    if (!data.size())
      return;
    std::vector<unsigned char> tmpData(data.size());
    memcpy(&tmpData[0], data.getDataBuffer(), data.size());
    _appendCharacters(result, tmpData, format);
  }

  void DRAWIOParser::_appendCharacters(librevenge::RVNGString &text,
                                       const std::vector<unsigned char> &characters,
                                       TextFormat format) {
    if (format == DRAWIO_TEXT_UTF16)
      return _appendCharacters(text, characters);
    if (format == DRAWIO_TEXT_UTF8) {
      // TODO: revisit for librevenge 0.1
      std::vector<unsigned char> buf;
      buf.reserve(characters.size() + 1);
      buf.assign(characters.begin(), characters.end());
      buf.push_back(0);
      text.append(reinterpret_cast<const char *>(buf.data()));
      return;
    }

    static const UChar32 symbolmap [] = {
      0x0020, 0x0021, 0x2200, 0x0023, 0x2203, 0x0025, 0x0026, 0x220D, // 0x20 ..
      0x0028, 0x0029, 0x2217, 0x002B, 0x002C, 0x2212, 0x002E, 0x002F,
      0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
      0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
      0x2245, 0x0391, 0x0392, 0x03A7, 0x0394, 0x0395, 0x03A6, 0x0393,
      0x0397, 0x0399, 0x03D1, 0x039A, 0x039B, 0x039C, 0x039D, 0x039F,
      0x03A0, 0x0398, 0x03A1, 0x03A3, 0x03A4, 0x03A5, 0x03C2, 0x03A9,
      0x039E, 0x03A8, 0x0396, 0x005B, 0x2234, 0x005D, 0x22A5, 0x005F,
      0xF8E5, 0x03B1, 0x03B2, 0x03C7, 0x03B4, 0x03B5, 0x03C6, 0x03B3,
      0x03B7, 0x03B9, 0x03D5, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BF,
      0x03C0, 0x03B8, 0x03C1, 0x03C3, 0x03C4, 0x03C5, 0x03D6, 0x03C9,
      0x03BE, 0x03C8, 0x03B6, 0x007B, 0x007C, 0x007D, 0x223C, 0x0020, // .. 0x7F
      0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,
      0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,
      0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,
      0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009E, 0x009f,
      0x20AC, 0x03D2, 0x2032, 0x2264, 0x2044, 0x221E, 0x0192, 0x2663, // 0xA0 ..
      0x2666, 0x2665, 0x2660, 0x2194, 0x2190, 0x2191, 0x2192, 0x2193,
      0x00B0, 0x00B1, 0x2033, 0x2265, 0x00D7, 0x221D, 0x2202, 0x2022,
      0x00F7, 0x2260, 0x2261, 0x2248, 0x2026, 0x23D0, 0x23AF, 0x21B5,
      0x2135, 0x2111, 0x211C, 0x2118, 0x2297, 0x2295, 0x2205, 0x2229,
      0x222A, 0x2283, 0x2287, 0x2284, 0x2282, 0x2286, 0x2208, 0x2209,
      0x2220, 0x2207, 0x00AE, 0x00A9, 0x2122, 0x220F, 0x221A, 0x22C5,
      0x00AC, 0x2227, 0x2228, 0x21D4, 0x21D0, 0x21D1, 0x21D2, 0x21D3,
      0x25CA, 0x3008, 0x00AE, 0x00A9, 0x2122, 0x2211, 0x239B, 0x239C,
      0x239D, 0x23A1, 0x23A2, 0x23A3, 0x23A7, 0x23A8, 0x23A9, 0x23AA,
      0xF8FF, 0x3009, 0x222B, 0x2320, 0x23AE, 0x2321, 0x239E, 0x239F,
      0x23A0, 0x23A4, 0x23A5, 0x23A6, 0x23AB, 0x23AC, 0x23AD, 0x0020  // .. 0xFE
    };

    UChar32  ucs4Character = 0;
    if (format == DRAWIO_TEXT_SYMBOL) { // SYMBOL
      for (unsigned char character : characters) {
        if (0x1e == ucs4Character)
          ucs4Character = 0xfffc;
        else if (character < 0x20)
          ucs4Character = 0x20;
        else
          ucs4Character = symbolmap[character - 0x20];
        appendUCS4(text, ucs4Character);
      }
    } else {
      UErrorCode status = U_ZERO_ERROR;
      UConverter *conv = nullptr;
      switch (format) {
      case DRAWIO_TEXT_JAPANESE:
        conv = ucnv_open("windows-932", &status);
        break;
      case DRAWIO_TEXT_KOREAN:
        conv = ucnv_open("windows-949", &status);
        break;
      case DRAWIO_TEXT_CHINESE_SIMPLIFIED:
        conv = ucnv_open("windows-936", &status);
        break;
      case DRAWIO_TEXT_CHINESE_TRADITIONAL:
        conv = ucnv_open("windows-950", &status);
        break;
      case DRAWIO_TEXT_GREEK:
        conv = ucnv_open("windows-1253", &status);
        break;
      case DRAWIO_TEXT_TURKISH:
        conv = ucnv_open("windows-1254", &status);
        break;
      case DRAWIO_TEXT_VIETNAMESE:
        conv = ucnv_open("windows-1258", &status);
        break;
      case DRAWIO_TEXT_HEBREW:
        conv = ucnv_open("windows-1255", &status);
        break;
      case DRAWIO_TEXT_ARABIC:
        conv = ucnv_open("windows-1256", &status);
        break;
      case DRAWIO_TEXT_BALTIC:
        conv = ucnv_open("windows-1257", &status);
        break;
      case DRAWIO_TEXT_RUSSIAN:
        conv = ucnv_open("windows-1251", &status);
        break;
      case DRAWIO_TEXT_THAI:
        conv = ucnv_open("windows-874", &status);
        break;
      case DRAWIO_TEXT_CENTRAL_EUROPE:
        conv = ucnv_open("windows-1250", &status);
        break;
      default:
        conv = ucnv_open("windows-1252", &status);
        break;
      }
      if (U_SUCCESS(status) && conv) {
        const auto *src = (const char *)&characters[0];
        const char *srcLimit = (const char *)src + characters.size();
        while (src < srcLimit) {
          ucs4Character = ucnv_getNextUChar(conv, &src, srcLimit, &status);
          if (U_SUCCESS(status) && U_IS_UNICODE_CHAR(ucs4Character)) {
            if (0x1e == ucs4Character)
              appendUCS4(text, 0xfffc);
            else
              appendUCS4(text, ucs4Character);
          }
        }
      }
      if (conv)
        ucnv_close(conv);
    }
  }

  void DRAWIOParser::_appendCharacters(librevenge::RVNGString &text,
                                       const std::vector<unsigned char> &characters) {
    UErrorCode status = U_ZERO_ERROR;
    UConverter *conv = ucnv_open("UTF-16LE", &status);

    if (U_SUCCESS(status) && conv) {
      const auto *src = (const char *)&characters[0];
      const char *srcLimit = (const char *)src + characters.size();
      while (src < srcLimit) {
        UChar32 ucs4Character = ucnv_getNextUChar(conv, &src, srcLimit, &status);
        if (U_SUCCESS(status) && U_IS_UNICODE_CHAR(ucs4Character))
          appendUCS4(text, ucs4Character);
      }
    }
    if (conv)
      ucnv_close(conv);
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
