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
        valid_ = false;
        return;
    }
    std::memcpy(dst_mac_.data(), data, 6);
    std::memcpy(src_mac_.data(), data + 6, 6);
    ethertype_ = (static_cast<uint16_t>(data[12]) << 8) | data[13];
    payload_ = data + HeaderSize;
    payload_len_ = length - HeaderSize;
    valid_ = true;
}

bool EthernetFrame::IsValid() const {
    return valid_;
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
    return MacToString(src_mac_);
}

std::string EthernetFrame::DstMac() const {
    return MacToString(dst_mac_);
}

EtherType EthernetFrame::Ethertype() const {
    switch (ethertype_) {
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
    return payload_;
}

size_t EthernetFrame::PayloadLength() const {
    return payload_len_;
}
} // namespace libpkt
