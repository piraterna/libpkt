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
#include <string>

namespace libpkt {
class Packet {
  public:
    Packet(const uint8_t* data, size_t length);
    virtual ~Packet();
    const uint8_t* Data() const;
    size_t Length() const;
    virtual std::string Summary() const;

  protected:
    const uint8_t* m_data;
    size_t m_length;
};

} // namespace libpkt
