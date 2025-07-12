// protocol.hpp
#pragma once

#include <cstdint>
#include <string>

namespace libpkt {

enum class Protocol : uint8_t {
    HOPOPT = 0,
    ICMP = 1,
    IGMP = 2,
    TCP = 6,
    UDP = 17,
    GRE = 47,
    ESP = 50,
    AH = 51,
    EIGRP = 88,
    OSPF = 89,
    SCTP = 132,
    ICMPv6 = 58,
    Unknown = 255
};

std::string ProtocolToString(Protocol proto);

} // namespace libpkt
