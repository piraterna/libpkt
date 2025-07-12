/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#include "libpkt/ipv4.hpp"

#include <arpa/inet.h>
#include <sstream>

namespace libpkt {
IPv4Packet::IPv4Packet(const uint8_t* data, size_t length)
    : m_data(data), m_length(length), m_valid(false) {
    if (length < MinHeaderSize)
        return;

    uint8_t version = data[0] >> 4;
    uint8_t ihl = data[0] & 0x0F;

    if (version != 4 || ihl < 5)
        return;

    size_t header_len = ihl * 4;
    if (length < header_len)
        return;

    m_valid = true;
}

bool IPv4Packet::IsValid() const {
    return m_valid;
}

uint8_t IPv4Packet::Version() const {
    return m_data[0] >> 4;
}

uint8_t IPv4Packet::HeaderLength() const {
    return (m_data[0] & 0x0F) * 4;
}

uint16_t IPv4Packet::TotalLength() const {
    return ntohs(*reinterpret_cast<const uint16_t*>(m_data + 2));
}

uint8_t IPv4Packet::ProtocolRaw() const {
    return m_data[9];
}

Protocol IPv4Packet::GetProtocol() const {
    if (!IsValid())
        return Protocol::Unknown;
    switch (ProtocolRaw()) {
    case 0:
        return Protocol::HOPOPT;
    case 1:
        return Protocol::ICMP;
    case 2:
        return Protocol::IGMP;
    case 6:
        return Protocol::TCP;
    case 17:
        return Protocol::UDP;
    case 47:
        return Protocol::GRE;
    case 50:
        return Protocol::ESP;
    case 51:
        return Protocol::AH;
    case 88:
        return Protocol::EIGRP;
    case 89:
        return Protocol::OSPF;
    case 132:
        return Protocol::SCTP;
    default:
        return Protocol::Unknown;
    }
}
std::string IPv4Packet::SrcAddress() const {
    return IPToString(*reinterpret_cast<const uint32_t*>(m_data + 12));
}

std::string IPv4Packet::DstAddress() const {
    return IPToString(*reinterpret_cast<const uint32_t*>(m_data + 16));
}

const uint8_t* IPv4Packet::Payload() const {
    return m_data + HeaderLength();
}

size_t IPv4Packet::PayloadLength() const {
    size_t header_len = HeaderLength();
    size_t total_len = TotalLength();
    return (total_len > header_len && total_len <= m_length) ? (total_len - header_len) : 0;
}

std::string IPv4Packet::IPToString(uint32_t ip) {
    struct in_addr addr;
    addr.s_addr = ip;
    return std::string(inet_ntoa(addr));
}
} // namespace libpkt
