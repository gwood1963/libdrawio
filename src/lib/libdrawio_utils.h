/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * This file is part of the libdrawio project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDED_LIBDRAWIO_UTILS_H
#define INCLUDED_LIBDRAWIO_UTILS_H

#include "librevenge/RVNGString.h"
#include <unicode/umachine.h>
#include <unicode/utf8.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <memory>
#include <string>

#include <boost/cstdint.hpp>

#include <librevenge-stream/librevenge-stream.h>
#include <librevenge/librevenge.h>

#if defined(HAVE_FUNC_ATTRIBUTE_FORMAT)
#define DRAWIO_ATTRIBUTE_PRINTF(fmt, arg) __attribute__((format(printf, fmt, arg)))
#else
#define DRAWIO_ATTRIBUTE_PRINTF(fmt, arg)
#endif

#if defined(HAVE_CLANG_ATTRIBUTE_FALLTHROUGH)
#  define DRAWIO_FALLTHROUGH [[clang::fallthrough]]
#elif defined(HAVE_GCC_ATTRIBUTE_FALLTHROUGH)
#  define DRAWIO_FALLTHROUGH __attribute__((fallthrough))
#else
#  define DRAWIO_FALLTHROUGH ((void) 0)
#endif

// do nothing with debug messages in a release compile
#ifdef DEBUG
namespace libdrawio
{
void debugPrint(const char *format, ...) DRAWIO_ATTRIBUTE_PRINTF(1, 2);
}

#define DRAWIO_DEBUG_MSG(M) libdrawio::debugPrint M
#define DRAWIO_DEBUG(M) M
#else
#define DRAWIO_DEBUG_MSG(M)
#define DRAWIO_DEBUG(M)
#endif

#define DRAWIO_NUM_ELEMENTS(array) (sizeof(array) / sizeof(array[0]))

namespace libdrawio
{

struct DRAWIODummyDeleter
{
  void operator()(void *) {}
};

uint8_t readU8(librevenge::RVNGInputStream *input, bool = false);
uint16_t readU16(librevenge::RVNGInputStream *input, bool bigEndian=false);
uint32_t readU32(librevenge::RVNGInputStream *input, bool bigEndian=false);
uint64_t readU64(librevenge::RVNGInputStream *input, bool bigEndian=false);

const unsigned char *readNBytes(librevenge::RVNGInputStream *input, unsigned long numBytes);

std::string readCString(librevenge::RVNGInputStream *input);
std::string readPascalString(librevenge::RVNGInputStream *input);

void skip(librevenge::RVNGInputStream *input, unsigned long numBytes);

void seek(librevenge::RVNGInputStream *input, unsigned long pos);
void seekRelative(librevenge::RVNGInputStream *input, long pos);

unsigned long getLength(librevenge::RVNGInputStream *input);

uint8_t readU8(std::shared_ptr<librevenge::RVNGInputStream> input, bool = false);
uint16_t readU16(std::shared_ptr<librevenge::RVNGInputStream> input, bool bigEndian=false);
uint32_t readU32(std::shared_ptr<librevenge::RVNGInputStream> input, bool bigEndian=false);
uint64_t readU64(std::shared_ptr<librevenge::RVNGInputStream> input, bool bigEndian=false);

const unsigned char *readNBytes(std::shared_ptr<librevenge::RVNGInputStream> input, unsigned long numBytes);

std::string readCString(std::shared_ptr<librevenge::RVNGInputStream> input);
std::string readPascalString(std::shared_ptr<librevenge::RVNGInputStream> input);

void skip(std::shared_ptr<librevenge::RVNGInputStream> input, unsigned long numBytes);

void seek(std::shared_ptr<librevenge::RVNGInputStream> input, unsigned long pos);
void seekRelative(std::shared_ptr<librevenge::RVNGInputStream> input, long pos);

unsigned long getLength(std::shared_ptr<librevenge::RVNGInputStream> input);

void appendUCS4(librevenge::RVNGString &text, UChar32 ucs4Character);

class EndOfStreamException
{
public:
  EndOfStreamException();
};

class GenericException
{
};

// parser exceptions

class FileAccessError
{
};

class PackageError
{
};

class ParseError
{
};

class XmlParserException
{
};    

class PasswordMismatch
{
};

class UnsupportedEncryption
{
};

class UnsupportedFormat
{
};

} // namespace libdrawio

#endif // INCLUDED_LIBDRAWIO_UTILS_H

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
