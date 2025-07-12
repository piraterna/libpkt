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

namespace libpkt::arp {
class Packet {
  public:
    static constexpr size_t HeaderSize = 28;

    Packet(const uint8_t* data, size_t length);

    bool IsValid() const;

    uint16_t HardwareType() const;
    uint16_t ProtocolType() const;
    uint8_t HardwareSize() const;
    uint8_t ProtocolSize() const;
    uint16_t Opcode() const;

    std::string SenderMAC() const;
    std::string SenderIP() const;
    std::string TargetMAC() const;
    std::string TargetIP() const;

    std::string Summary() const;

  private:
    const uint8_t* data_;
    size_t length_;
    bool valid_;

    std::string MACToString(const uint8_t* mac) const;
    std::string IPToString(const uint8_t* ip) const;
};
} // namespace libpkt::arp
