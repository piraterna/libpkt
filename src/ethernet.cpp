/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#include "libpkt/ethernet.hpp"

#include <cstring>
#include <iomanip>
#include <sstream>

namespace libpkt {
EthernetFrame::EthernetFrame(const uint8_t* data, size_t length) {
    if (length < HeaderSize) {
        m_valid = false;
        return;
    }
    std::memcpy(m_dst_mac.data(), data, 6);
    std::memcpy(m_src_mac.data(), data + 6, 6);
    m_ethertype = (static_cast<uint16_t>(data[12]) << 8) | data[13];
    m_payload = data + HeaderSize;
    m_payload_len = length - HeaderSize;
    m_valid = true;
}

bool EthernetFrame::IsValid() const {
    return m_valid;
}

std::string EthernetFrame::MacToString(const std::array<uint8_t, 6>& mac) const {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < mac.size(); ++i) {
        oss << std::setw(2) << static_cast<int>(mac[i]);
        if (i != mac.size() - 1)
            oss << ":";
    }
    return oss.str();
}

std::string EthernetFrame::SrcMac() const {
    return MacToString(m_src_mac);
}

std::string EthernetFrame::DstMac() const {
    return MacToString(m_dst_mac);
}

EtherType EthernetFrame::Ethertype() const {
    switch (m_ethertype) {
    case 0x0800:
        return EtherType::IPv4;
    case 0x0806:
        return EtherType::ARP;
    case 0x0842:
        return EtherType::WakeOnLAN;
    case 0x86DD:
        return EtherType::IPv6;
    case 0x8100:
        return EtherType::VLAN;
    case 0x88CC:
        return EtherType::LLDP;
    default:
        return EtherType::Unknown;
    }
}
const uint8_t* EthernetFrame::Payload() const {
    return m_payload;
}

size_t EthernetFrame::PayloadLength() const {
    return m_payload_len;
}
} // namespace libpkt
