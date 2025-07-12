/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#pragma once

#include <cstddef>
#include <cstdint>

namespace libpkt::checksum {
// Compute IP checksum (RFC 1071)
uint16_t IPChecksum(const uint8_t* data, size_t length);
} // namespace libpkt::checksum
