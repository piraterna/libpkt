/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#pragma once

#include <cstdint>
#include <string>

namespace libpkt::icmp {
class Packet {
  public:
    static constexpr size_t MinHeaderSize = 8;

    Packet(const uint8_t* data, size_t length);

    bool IsValid() const;

    uint8_t Type() const;
    uint8_t Code() const;
    uint16_t Checksum() const;

    std::string Summary() const;

  private:
    const uint8_t* m_data;
    size_t m_length;
    bool m_valid;
};
} // namespace libpkt::icmp
