/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#include "libpkt/arp.hpp"

#include <arpa/inet.h>
#include <iomanip>
#include <sstream>

namespace libpkt::arp {
#pragma pack(push, 1)
struct ArpHeader {
    uint16_t hw_type;
    uint16_t proto_type;
    uint8_t hw_size;
    uint8_t proto_size;
    uint16_t opcode;
    uint8_t sender_mac[6];
    uint8_t sender_ip[4];
    uint8_t target_mac[6];
    uint8_t target_ip[4];
};
#pragma pack(pop)

Packet::Packet(const uint8_t* data, size_t length) : data_(data), length_(length), valid_(false) {
    if (length >= sizeof(ArpHeader)) {
        valid_ = true;
    }
}

bool Packet::IsValid() const {
    return valid_;
}

uint16_t Packet::HardwareType() const {
    if (!valid_)
        return 0;
    auto hdr = reinterpret_cast<const ArpHeader*>(data_);
    return ntohs(hdr->hw_type);
}

uint16_t Packet::ProtocolType() const {
    if (!valid_)
        return 0;
    auto hdr = reinterpret_cast<const ArpHeader*>(data_);
    return ntohs(hdr->proto_type);
}

uint8_t Packet::HardwareSize() const {
    if (!valid_)
        return 0;
    auto hdr = reinterpret_cast<const ArpHeader*>(data_);
    return hdr->hw_size;
}

uint8_t Packet::ProtocolSize() const {
    if (!valid_)
        return 0;
    auto hdr = reinterpret_cast<const ArpHeader*>(data_);
    return hdr->proto_size;
}

uint16_t Packet::Opcode() const {
    if (!valid_)
        return 0;
    auto hdr = reinterpret_cast<const ArpHeader*>(data_);
    return ntohs(hdr->opcode);
}

std::string Packet::MACToString(const uint8_t* mac) const {
    std::ostringstream oss;
    for (int i = 0; i < 6; ++i) {
        if (i)
            oss << ":";
        oss << std::hex << std::setw(2) << std::setfill('0') << (int) mac[i];
    }
    return oss.str();
}

std::string Packet::IPToString(const uint8_t* ip) const {
    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, ip, buf, sizeof(buf));
    return std::string(buf);
}

std::string Packet::SenderMAC() const {
    if (!valid_)
        return {};
    auto hdr = reinterpret_cast<const ArpHeader*>(data_);
    return MACToString(hdr->sender_mac);
}

std::string Packet::SenderIP() const {
    if (!valid_)
        return {};
    auto hdr = reinterpret_cast<const ArpHeader*>(data_);
    return IPToString(hdr->sender_ip);
}

std::string Packet::TargetMAC() const {
    if (!valid_)
        return {};
    auto hdr = reinterpret_cast<const ArpHeader*>(data_);
    return MACToString(hdr->target_mac);
}

std::string Packet::TargetIP() const {
    if (!valid_)
        return {};
    auto hdr = reinterpret_cast<const ArpHeader*>(data_);
    return IPToString(hdr->target_ip);
}

std::string Packet::Summary() const {
    if (!valid_)
        return "Invalid ARP Packet";
    std::ostringstream oss;
    oss << "ARP Packet: Opcode=" << Opcode() << ", Sender=" << SenderMAC() << "/" << SenderIP()
        << ", Target=" << TargetMAC() << "/" << TargetIP();
    return oss.str();
}
} // namespace libpkt::arp
