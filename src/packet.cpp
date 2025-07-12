/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */

#include "libpkt/packet.hpp"

#include <string>

namespace libpkt {
Packet::Packet(const uint8_t* data, size_t length) : m_data(data), m_length(length) {}
Packet::~Packet() = default;

const uint8_t* Packet::Data() const {
    return m_data;
}

size_t Packet::Length() const {
    return m_length;
}

std::string Packet::Summary() const {
    return "Generic Packet: length = " + std::to_string(m_length);
}
} // namespace libpkt
