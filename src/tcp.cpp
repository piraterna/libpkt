/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#include "libpkt/tcp.hpp"

#include <arpa/inet.h>
#include <iomanip>
#include <sstream>

namespace libpkt::tcp {
#pragma pack(push, 1)
struct TcpHeader {
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq_num;
    uint32_t ack_num;
    uint8_t m_dataoffset_reserved;
    uint8_t flags;
    uint16_t window;
    uint16_t checksum;
    uint16_t urgent_pointer;
};
#pragma pack(pop)

Packet::Packet(const uint8_t* data, size_t length) : libpkt::Packet(data, length), m_valid(false) {
    if (length >= sizeof(TcpHeader)) {
        m_valid = true;
    }
}

Packet::~Packet() = default;

uint16_t Packet::SrcPort() const {
    if (!m_valid)
        return 0;
    auto hdr = reinterpret_cast<const TcpHeader*>(m_data);
    return ntohs(hdr->src_port);
}

uint16_t Packet::DstPort() const {
    if (!m_valid)
        return 0;
    auto hdr = reinterpret_cast<const TcpHeader*>(m_data);
    return ntohs(hdr->dst_port);
}

uint32_t Packet::SeqNum() const {
    if (!m_valid)
        return 0;
    auto hdr = reinterpret_cast<const TcpHeader*>(m_data);
    return ntohl(hdr->seq_num);
}

uint32_t Packet::AckNum() const {
    if (!m_valid)
        return 0;
    auto hdr = reinterpret_cast<const TcpHeader*>(m_data);
    return ntohl(hdr->ack_num);
}

uint8_t Packet::DataOffset() const {
    if (!m_valid)
        return 0;
    auto hdr = reinterpret_cast<const TcpHeader*>(m_data);
    return (hdr->m_dataoffset_reserved >> 4) * 4; // bytes
}

uint8_t Packet::Flags() const {
    if (!m_valid)
        return 0;
    auto hdr = reinterpret_cast<const TcpHeader*>(m_data);
    return hdr->flags;
}

uint16_t Packet::Window() const {
    if (!m_valid)
        return 0;
    auto hdr = reinterpret_cast<const TcpHeader*>(m_data);
    return ntohs(hdr->window);
}

std::string Packet::Summary() const {
    if (!m_valid) {
        return "Invalid TCP Packet";
    }
    std::ostringstream oss;
    oss << "TCP Packet: SrcPort=" << SrcPort() << ", DstPort=" << DstPort()
        << ", SeqNum=" << SeqNum() << ", AckNum=" << AckNum() << ", Flags=0x" << std::hex
        << std::setw(2) << std::setfill('0') << static_cast<int>(Flags()) << ", Window=" << std::dec
        << Window();
    return oss.str();
}

bool Packet::IsValid() const {
    return m_valid;
}

} // namespace libpkt::tcp
