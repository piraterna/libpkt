#include "libpkt/arp.hpp"
#include "libpkt/ethernet.hpp"
#include "libpkt/icmp.hpp"
#include "libpkt/interface.hpp"
#include "libpkt/ipv4.hpp"
#include "libpkt/protocol.hpp"
#include "libpkt/tcp.hpp"
#include "libpkt/udp.hpp"
#include "libpkt/utils/checksum.hpp"

#include <arpa/inet.h>
#include <atomic>
#include <csignal>
#include <iostream>
#include <vector>

std::atomic<bool> running(true);

void signal_handler(int) {
    running = false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <interface>" << std::endl;
        return 1;
    }

    std::signal(SIGINT, signal_handler);

    libpkt::Interface iface(argv[1]);
    if (!iface.Open()) {
        std::cerr << "Failed to open interface: " << iface.Name() << std::endl;
        return 1;
    }

    std::cout << "Listening on interface: " << iface.Name() << std::endl;

    constexpr size_t buffer_size = 65536;
    std::vector<uint8_t> buffer(buffer_size);

    while (running) {
        ssize_t received = iface.Receive(buffer.data(), buffer.size());
        if (received <= 0) {
            std::cerr << "Receive error or connection closed." << std::endl;
            break;
        }

        libpkt::EthernetFrame ethFrame(buffer.data(), static_cast<size_t>(received));
        if (!ethFrame.IsValid())
            continue;

        std::cout << "Ethernet: " << ethFrame.SrcMac() << " -> " << ethFrame.DstMac()
                  << ", EtherType: 0x" << std::hex << static_cast<uint16_t>(ethFrame.Ethertype())
                  << std::dec << "\n";

        auto ethertype = ethFrame.Ethertype();

        if (ethertype == libpkt::EtherType::IPv4) {
            libpkt::IPv4Packet ipPkt(ethFrame.Payload(), ethFrame.PayloadLength());
            if (!ipPkt.IsValid())
                continue;

            // Verify IPv4 checksum
            uint16_t checksum =
                libpkt::checksum::IPChecksum(ethFrame.Payload(), ipPkt.HeaderLength());
            if (checksum != 0) {
                std::cerr << "Warning: IPv4 header checksum invalid (computed: 0x" << std::hex
                          << checksum << std::dec << ")\n";
            }

            std::cout << "IPv4: " << ipPkt.SrcAddress() << " -> " << ipPkt.DstAddress()
                      << ", Protocol: " << static_cast<int>(ipPkt.GetProtocol()) << " ("
                      << libpkt::ProtocolToString(ipPkt.GetProtocol()) << ")\n";

            switch (ipPkt.GetProtocol()) {
            case libpkt::Protocol::TCP: {
                libpkt::tcp::Packet tcpPkt(ipPkt.Payload(), ipPkt.PayloadLength());
                if (tcpPkt.IsValid()) {
                    std::cout << tcpPkt.Summary() << std::endl;
                }
                break;
            }
            case libpkt::Protocol::UDP: {
                libpkt::udp::Packet udpPkt(ipPkt.Payload(), ipPkt.PayloadLength());
                if (udpPkt.IsValid()) {
                    std::cout << udpPkt.Summary() << std::endl;
                }
                break;
            }
            case libpkt::Protocol::ICMP: {
                libpkt::icmp::Packet icmpPkt(ipPkt.Payload(), ipPkt.PayloadLength());
                if (icmpPkt.IsValid()) {
                    std::cout << icmpPkt.Summary() << std::endl;
                }
                break;
            }
            default:
                std::cout << "Unknown IPv4 Protocol: " << static_cast<int>(ipPkt.GetProtocol())
                          << "\n";
            }
        } else if (ethertype == libpkt::EtherType::IPv6) {
            std::cout << "IPv6 packets are not yet supported by libpkt.\n";
        } else if (ethertype == libpkt::EtherType::ARP) {
            libpkt::arp::Packet arpPkt(ethFrame.Payload(), ethFrame.PayloadLength());
            if (arpPkt.IsValid()) {
                std::cout << arpPkt.Summary() << std::endl;
            }
        } else {
            std::cout << "Unhandled EtherType: 0x" << std::hex << static_cast<uint16_t>(ethertype)
                      << std::dec << "\n";
        }
    }

    iface.Close();
    std::cout << "Exiting..." << std::endl;
    return 0;
}
