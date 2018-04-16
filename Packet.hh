//
// Created by klh on 4/10/18.
//

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
    std::vector<uint8_t> m_buf;
  private:
    int m_readPos = 0;
  };
}

#endif //LIB_B6_DEMO_PACKET_HH
