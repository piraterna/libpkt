/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace libpkt {

enum class EtherType : uint16_t {
    IPv4 = 0x0800,
    ARP = 0x0806,
    WakeOnLAN = 0x0842,
    IPv6 = 0x86DD,
    VLAN = 0x8100,
    LLDP = 0x88CC,
    Unknown = 0xFFFF
};

class EthernetFrame {
  public:
    static constexpr size_t HeaderSize = 14;

    EthernetFrame(const uint8_t* data, size_t length);

    bool IsValid() const;

    std::string SrcMac() const;
    std::string DstMac() const;
    uint16_t EthertypeRaw() const;
    EtherType Ethertype() const;

    const uint8_t* Payload() const;
    size_t PayloadLength() const;

  private:
    std::array<uint8_t, 6> dst_mac_;
    std::array<uint8_t, 6> src_mac_;
    uint16_t ethertype_;
    const uint8_t* payload_;
    size_t payload_len_;
    bool valid_;

    std::string MacToString(const std::array<uint8_t, 6>& mac) const;
};

} // namespace libpkt
