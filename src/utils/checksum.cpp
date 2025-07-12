/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */                                                                                                \
#include "libpkt/utils/checksum.hpp"
#include <arpa/inet.h>
#include <cstring>

namespace libpkt::checksum {

uint16_t IPChecksum(const uint8_t* data, size_t length) {
    uint32_t sum = 0;
    for (size_t i = 0; i < length / 2; ++i) {
        uint16_t word;
        memcpy(&word, data + i * 2, 2);
        sum += ntohs(word);
    }
    if (length & 1) {
        sum += data[length - 1] << 8;
    }
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return htons(~sum);
}
} // namespace libpkt::checksum
