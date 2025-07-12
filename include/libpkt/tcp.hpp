/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#pragma once

#include "packet.hpp"

#include <cstdint>
#include <string>

namespace libpkt::tcp {

class Packet : public libpkt::Packet {
  public:
    Packet(const uint8_t* data, size_t length);
    ~Packet() override;

    uint16_t SrcPort() const;
    uint16_t DstPort() const;
    uint32_t SeqNum() const;
    uint32_t AckNum() const;
    uint8_t DataOffset() const;
    uint8_t Flags() const;
    uint16_t Window() const;
    bool IsValid() const;

    std::string Summary() const override;

  private:
    bool m_valid;
};

} // namespace libpkt::tcp
