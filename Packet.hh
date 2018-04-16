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

#ifndef LIB_B6_DEMO_PACKET_HH
#define LIB_B6_DEMO_PACKET_HH

#include <cstdint>
#include <vector>
#include <string>

namespace b6 {
  class Packet {
  public:
    Packet() = default;
    explicit Packet(std::vector<uint8_t> buf) { m_buf = std::move(buf); };

    inline void skip(int count) { m_readPos += count; };
    inline uint8_t readU8() { return m_buf[m_readPos++]; };
    inline uint16_t readU16() { return (readU8() * static_cast<uint16_t>(256)) + readU8(); };
    std::string readString(size_t len);

    void writeUA(std::vector<uint8_t> buf);
    inline void writeU8(uint8_t val) { m_buf.push_back(val); }
    inline void writeU16(uint16_t val) { writeU8(static_cast<uint8_t>(val >> 8)); writeU8(static_cast<uint8_t>(val & 0xff)); };
    void writeChecksum();

    uint8_t* getBuffer() { return &m_buf[0]; }
    int getSize() { return static_cast<int>(m_buf.size()); }
  private:
    std::vector<uint8_t> m_buf;
    int m_readPos = 0;
  };
}

#endif //LIB_B6_DEMO_PACKET_HH
