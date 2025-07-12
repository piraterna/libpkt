/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */                                                                                                \
#include "libpkt/protocol.hpp"

namespace libpkt {

std::string ProtocolToString(Protocol proto) {
    switch (proto) {
    case Protocol::HOPOPT:
        return "HOPOPT";
    case Protocol::ICMP:
        return "ICMP";
    case Protocol::IGMP:
        return "IGMP";
    case Protocol::TCP:
        return "TCP";
    case Protocol::UDP:
        return "UDP";
    case Protocol::GRE:
        return "GRE";
    case Protocol::ESP:
        return "ESP";
    case Protocol::AH:
        return "AH";
    case Protocol::EIGRP:
        return "EIGRP";
    case Protocol::OSPF:
        return "OSPF";
    case Protocol::SCTP:
        return "SCTP";
    case Protocol::ICMPv6:
        return "ICMPv6";
    default:
        return "Unknown";
    }
}
} // namespace libpkt
