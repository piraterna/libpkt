/*
 * ============================================================================
 * libpkt - A low-level C++ networking library for Linux.
 * Apache License 2.0 (see LICENSE file or
 * https://www.apache.org/licenses/LICENSE-2.0)
 * ============================================================================
 */
#pragma once

#include <cstdint>
#include <string>

namespace libpkt {
class Interface {
  public:
    explicit Interface(const std::string& ifaceName);
    ~Interface();

    bool Open();
    void Close();
    bool IsOpen() const;

    ssize_t Receive(uint8_t* buffer, size_t length);
    std::string Name() const { return m_ifaceName; }

    Interface(const Interface&) = delete;
    Interface& operator=(const Interface&) = delete;

  private:
    std::string m_ifaceName;
    int m_sockFd;
};
} // namespace libpkt