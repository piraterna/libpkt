/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#include "libpkt/interface.hpp"

#include <cstring>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

namespace libpkt {
Interface::Interface(const std::string& ifaceName) : m_ifaceName(ifaceName), m_sockFd(-1) {}

Interface::~Interface() {
    Close();
}

bool Interface::Open() {
    m_sockFd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (m_sockFd < 0) {
        return false;
    }

    struct ifreq ifr{};
    std::strncpy(ifr.ifr_name, m_ifaceName.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    if (ioctl(m_sockFd, SIOCGIFINDEX, &ifr) < 0) {
        Close();
        return false;
    }

    struct sockaddr_ll sll{};
    sll.sll_family = AF_PACKET;
    sll.sll_protocol = htons(ETH_P_ALL);
    sll.sll_ifindex = ifr.ifr_ifindex;

    if (bind(m_sockFd, reinterpret_cast<struct sockaddr*>(&sll), sizeof(sll)) < 0) {
        Close();
        return false;
    }

    return true;
}

void Interface::Close() {
    if (m_sockFd != -1) {
        ::close(m_sockFd);
        m_sockFd = -1;
    }
}

bool Interface::IsOpen() const {
    return m_sockFd != -1;
}

ssize_t Interface::Receive(uint8_t* buffer, size_t length) {
    if (m_sockFd == -1) {
        return -1;
    }
    return ::recv(m_sockFd, buffer, length, 0);
}
} // namespace libpkt
