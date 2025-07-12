/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#pragma once

#include "protocol.hpp"

#include <cstdint>
#include <string>

namespace libpkt {
class IPv4Packet {
  public:
    static constexpr size_t MinHeaderSize = 20;

    IPv4Packet(const uint8_t* data, size_t length);

    bool IsValid() const;

    uint8_t Version() const;
    uint8_t HeaderLength() const;
    uint16_t TotalLength() const;
    uint8_t ProtocolRaw() const;
    Protocol GetProtocol() const;

    std::string SrcAddress() const;
    std::string DstAddress() const;

    const uint8_t* Payload() const;
    size_t PayloadLength() const;

  private:
    const uint8_t* m_data;
    size_t m_length;
    bool m_valid;

    static std::string IPToString(uint32_t ip);
};

} // namespace libpkt
