# libpkt

libpkt — A low-level C++ networking library for Linux.

## Overview

**libpkt** is a low-level C++ library for parsing and handling network packets on Linux. It provides efficient utilities for dissecting Ethernet frames, ARP, IPv4, TCP, UDP, and ICMP packets, with a focus on raw packet capture and decoding.

## Features

- Parse raw Ethernet frames
- ARP packet parsing
- IPv4 packet parsing
- TCP and UDP packet parsing
- ICMP packet parsing
- Simple network interface capture API (using raw sockets)
- Header and payload extraction for supported protocols


## Protocol & Feature Support Roadmap

| Protocol/Feature    | Supported | Notes / API | 
|---------------------|:---------:|-------------|
| Ethernet (802.3)    |    ✅     | `libpkt::EthernetFrame` ([ethernet.hpp](include/libpkt/ethernet.hpp)) |
| ARP                 |    ✅     | `libpkt::arp::Packet` ([arp.hpp](include/libpkt/arp.hpp)) |
| IPv4                |    ✅     | `libpkt::IPv4Packet` ([ipv4.hpp](include/libpkt/ipv4.hpp)) |
| TCP                 |    ✅     | `libpkt::tcp::Packet` ([tcp.hpp](include/libpkt/tcp.hpp)) |
| UDP                 |    ✅     | `libpkt::udp::Packet` ([udp.hpp](include/libpkt/udp.hpp)) |
| ICMP                |    ✅     | `libpkt::icmp::Packet` ([icmp.hpp](include/libpkt/icmp.hpp)) |
| Network Interface   |    ✅     | `libpkt::Interface` ([interface.hpp](include/libpkt/interface.hpp)) |
| IGMP                |    ⚠️     | No implementation |
| VLAN                |    ⚠️     | No parser |
| IPv6                |    ❌     | Not supported |
| ICMPv6              |    ⚠️     | No implementation |
| GRE / ESP / AH      |    ⚠️     | No implementation |
| EIGRP / OSPF / SCTP |    ⚠️     | No implementation |
| LLDP                |    ⚠️     | No parser |
| Wake-on-LAN         |    ⚠️     | No parser |
(The rest are a TODO)

**Legend:**  
✅ - Supported and parsed  
⚠️ - Partially recognized  
❌ - Not supported


## Example Usage

```cpp
#include "libpkt/ethernet.hpp"
#include "libpkt/ipv4.hpp"
#include "libpkt/tcp.hpp"
#include "libpkt/udp.hpp"
#include "libpkt/icmp.hpp"
#include "libpkt/arp.hpp"
#include "libpkt/interface.hpp"

// Open a network interface and receive packets
libpkt::Interface iface("eth0");
if (iface.Open()) {
    uint8_t buffer[65536];
    ssize_t len = iface.Receive(buffer, sizeof(buffer));
    if (len > 0) {
        libpkt::EthernetFrame eth(buffer, len);
        if (eth.IsValid()) {
            if (eth.Ethertype() == libpkt::EtherType::IPv4) {
                libpkt::IPv4Packet ip(eth.Payload(), eth.PayloadLength());
                if (ip.IsValid()) {
                    // Handle TCP/UDP/ICMP etc.
                }
            }
        }
    }
    iface.Close();
}
```

See [`examples/all.cpp`](examples/all.cpp) for a full working example that prints packet summaries (only supported protocols and features).


## License

Apache License 2.0. See [LICENSE](LICENSE) or [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0) for details.