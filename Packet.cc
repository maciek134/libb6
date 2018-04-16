/* Copyright © 2018, Maciej Sopyło <me@klh.io>
 *
 * This file is part of libb6.
 *
 *  libb6 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libb6 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libb6.  If not, see <http://www.gnu.org/licenses/>.
 */

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