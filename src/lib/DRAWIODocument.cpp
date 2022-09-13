/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * This file is part of the libdrawio project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <cstring>
#include <memory>

#include <librevenge-stream/librevenge-stream.h>

#include <libdrawio/libdrawio.h>

#include "libdrawio_utils.h"

using librevenge::RVNGInputStream;

using std::equal;
using std::shared_ptr;

namespace libdrawio
{

DRAWIOAPI DRAWIODocument::Confidence DRAWIODocument::isSupported(librevenge::RVNGInputStream *const input, Type *const type) try
{
  if (type)
    *type = TYPE_UNKNOWN;

  return CONFIDENCE_NONE;
}
catch (...)
{
  return CONFIDENCE_NONE;
}

DRAWIOAPI DRAWIODocument::Result DRAWIODocument::parse(librevenge::RVNGInputStream *const input, librevenge::RVNGDrawingInterface *const document, const char *const)
{
  Type type;
  Confidence confidence = isSupported(input, &type);
  if (CONFIDENCE_NONE == confidence)
    return RESULT_UNSUPPORTED_FORMAT;
  else if (CONFIDENCE_SUPPORTED_PART == confidence)
    return RESULT_UNSUPPORTED_FORMAT;
  else if (CONFIDENCE_UNSUPPORTED_ENCRYPTION == confidence)
    return RESULT_UNSUPPORTED_ENCRYPTION;

  return parse(input, document, type);
}

DRAWIOAPI DRAWIODocument::Result DRAWIODocument::parse(librevenge::RVNGInputStream *const input, librevenge::RVNGDrawingInterface *const document, const DRAWIODocument::Type type, const char *const) try
{
  // sanity check
  if (DRAWIODocument::TYPE_UNKNOWN == type)
    return DRAWIODocument::RESULT_UNSUPPORTED_FORMAT;
  if (DRAWIODocument::TYPE_RESERVED1 <= type)
    return DRAWIODocument::RESULT_UNSUPPORTED_FORMAT;

  const std::shared_ptr<RVNGInputStream> input_(input, DRAWIODummyDeleter());

  input_->seek(0, librevenge::RVNG_SEEK_SET);

  return RESULT_UNKNOWN_ERROR;
}
catch (const FileAccessError &)
{
  return RESULT_FILE_ACCESS_ERROR;
}
catch (const PackageError &)
{
  return RESULT_PACKAGE_ERROR;
}
catch (const PasswordMismatch &)
{
  return RESULT_PASSWORD_MISMATCH;
}
catch (const UnsupportedEncryption &)
{
  return RESULT_UNSUPPORTED_ENCRYPTION;
}
catch (const UnsupportedFormat &)
{
  return RESULT_UNSUPPORTED_FORMAT;
}
catch (...)
{
  return RESULT_UNKNOWN_ERROR;
}

} // namespace libdrawio

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
