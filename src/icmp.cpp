/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#include "libpkt/icmp.hpp"

#include <arpa/inet.h>
#include <iomanip>
#include <sstream>

namespace libpkt::icmp {
#pragma pack(push, 1)
struct IcmpHeader {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint32_t rest; // varies by type
};
#pragma pack(pop)

Packet::Packet(const uint8_t* data, size_t length) : data_(data), length_(length), valid_(false) {
    if (length >= sizeof(IcmpHeader)) {
        valid_ = true;
    }
}

bool Packet::IsValid() const {
    return valid_;
}

uint8_t Packet::Type() const {
    if (!valid_)
        return 0;
    auto hdr = reinterpret_cast<const IcmpHeader*>(data_);
    return hdr->type;
}

uint8_t Packet::Code() const {
    if (!valid_)
        return 0;
    auto hdr = reinterpret_cast<const IcmpHeader*>(data_);
    return hdr->code;
}

uint16_t Packet::Checksum() const {
    if (!valid_)
        return 0;
    auto hdr = reinterpret_cast<const IcmpHeader*>(data_);
    return ntohs(hdr->checksum);
}

std::string Packet::Summary() const {
    if (!valid_)
        return "Invalid ICMP Packet";
    std::ostringstream oss;
    oss << "ICMP Packet: Type=" << (int) Type() << ", Code=" << (int) Code();
    return oss.str();
}
} // namespace libpkt::icmp
