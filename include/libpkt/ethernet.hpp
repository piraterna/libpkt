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

// EthernetType, values gathered from: https://en.wikipedia.org/wiki/EtherType#Values
// NOTE 1: Not all values are added
// NOTE 2: Not all these has a libpkt implementation
enum class EtherType : uint16_t {
    IPv4 = 0x0800, // Internet Protocol version 4
    ARP = 0x0806,  // Adress Resolution Protocol
    WOL = 0x0842,  // Wake-On-LAN
    VLAN = 0x8100, // VLAN-tagged frame (IEE 802.1Q)
    IPv6 = 0x86DD, // Internet Protocol version 6
    LLDP = 0x88CC, // Link Layer Discovery Protocol
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
    std::array<uint8_t, 6> m_dst_mac;
    std::array<uint8_t, 6> m_src_mac;
    uint16_t m_ethertype;
    const uint8_t* m_payload;
    size_t m_payload_len;
    bool m_valid;

    std::string MacToString(const std::array<uint8_t, 6>& mac) const;
};

} // namespace libpkt
