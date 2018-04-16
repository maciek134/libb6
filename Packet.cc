//
// Created by klh on 4/15/18.
//

#include "Packet.hh"

namespace b6 {
  std::string Packet::readString(size_t len) {
    std::string ret;
    for (int i = 0; i < len; i++) {
      ret += (char)readU8();
    }
    return ret;
  }

  void Packet::writeUA(std::vector<uint8_t> buf) {
    m_buf.insert(m_buf.end(), buf.begin(), buf.end());
  }

  void Packet::writeChecksum() {
    // chinese checksum :D
    unsigned int sum = 0;
    for (auto it = m_buf.begin() + 2; it != m_buf.end(); it++) {
      sum += *it;
    }
    writeUA({ static_cast<uint8_t>(sum & 0xff), 0xff, 0xff });
  }
}