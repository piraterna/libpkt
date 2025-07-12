/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#include "libpkt/udp.hpp"

#include <arpa/inet.h>
#include <iomanip>
#include <sstream>

namespace libpkt::udp {

#pragma pack(push, 1)
struct UdpHeader {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
};
#pragma pack(pop)

Packet::Packet(const uint8_t* data, size_t length) : libpkt::Packet(data, length), m_valid(false) {
    if (length >= sizeof(UdpHeader)) {
        m_valid = true;
    }
}

Packet::~Packet() = default;

uint16_t Packet::SrcPort() const {
    if (!m_valid)
        return 0;
    auto hdr = reinterpret_cast<const UdpHeader*>(m_data);
    return ntohs(hdr->src_port);
}

uint16_t Packet::DstPort() const {
    if (!m_valid)
        return 0;
    auto hdr = reinterpret_cast<const UdpHeader*>(m_data);
    return ntohs(hdr->dst_port);
}

std::string Packet::Summary() const {
    if (!m_valid) {
        return "Invalid UDP Packet";
    }
    std::ostringstream oss;
    oss << "UDP Packet: SrcPort=" << SrcPort() << ", DstPort=" << DstPort();
    return oss.str();
}

bool Packet::IsValid() const {
    return m_valid;
}

} // namespace libpkt::udp
