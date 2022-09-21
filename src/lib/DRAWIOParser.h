/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef DRAWIOPARSER_H
#define DRAWIOPARSER_H

#include "DRAWIOPage.h"
#include "DRAWIOTypes.h"
#include "DRAWIOUserObject.h"
#include "MXCell.h"
#include "MXGeometry.h"
#include "libdrawio_xml.h"
#include "librevenge-stream/librevenge-stream.h"
#include "librevenge/RVNGString.h"
#include "librevenge/librevenge.h"
#include <libxml/xmlreader.h>
#include <map>
#include <vector>

namespace libdrawio {
  class DRAWIOParser {
  public:
    DRAWIOParser(librevenge::RVNGInputStream *input,
                 librevenge::RVNGDrawingInterface *painter);
    ~DRAWIOParser();
    bool parseMain();
  private:
    bool _processXmlDocument(librevenge::RVNGInputStream *input);
    void _processXmlNode(xmlTextReaderPtr reader);
    void _readCell(xmlTextReaderPtr reader);
    void _readObject(xmlTextReaderPtr reader);
    void _readGeometry(xmlTextReaderPtr reader);
    void _readPoint(xmlTextReaderPtr reader);
    void _readGraphModel(xmlTextReaderPtr reader);
    void _startPage(xmlTextReaderPtr reader);
    void _flushCell();
    void _flushGeometry();
    void _endPage();
    void _endDocument();
    int _getElementToken(xmlTextReaderPtr reader);
    int _getElementDepth(xmlTextReaderPtr reader);
    void _handleLevelChange(unsigned level);
    void _convertDataToString(librevenge::RVNGString &result,
                              const librevenge::RVNGBinaryData &data,
                              TextFormat format);
    void _appendCharacters(librevenge::RVNGString &text,
                           const std::vector<unsigned char> &characters,
                           TextFormat format);
    void _appendCharacters(librevenge::RVNGString &text,
                           const std::vector<unsigned char> &characters);
    xmlChar *_readStringData(xmlTextReaderPtr reader);
    int _readStringData(DRAWIOName &text, xmlTextReaderPtr reader);
    int _readDoubleData(double &value, xmlTextReaderPtr reader);
    int _readBoolData(bool &value, xmlTextReaderPtr reader);
    librevenge::RVNGInputStream *m_input;
    librevenge::RVNGDrawingInterface *m_painter;
    DRAWIOUserObject m_value;
    MXCell m_cell;
    MXGeometry m_geometry;
    MXPoint m_point;
    DRAWIOPage m_current_page;
    std::vector<DRAWIOPage> m_pages;
    bool m_objectStarted, m_cellStarted, m_geometryStarted;
    bool m_in_points_list;
    unsigned m_current_level;
    XMLErrorWatcher *m_watcher;
    std::map<librevenge::RVNGString, MXCell> m_id_map;

    DRAWIOParser(const DRAWIOParser &parser);
    DRAWIOParser &operator=(const DRAWIOParser &parser);
  };
}

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
