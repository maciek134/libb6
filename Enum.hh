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

#ifndef LIB_B6_DEMO_ENUM_HH
#define LIB_B6_DEMO_ENUM_HH

#include <cstdint>

namespace b6 {
  enum class CMD : uint8_t {
    GET_DEV_INFO            = 0x57,
    GET_SYS_INFO            = 0x5a,
    GET_CHARGE_INFO         = 0x55,
    UNK1                    = 0x5f,

    STOP_CHARGING           = 0xfe,
  };

  enum class CHARGING_MODE_LI : uint8_t {
    STANDARD                = 0x00,
    DISCHARGE               = 0x01,
    STORAGE                 = 0x02,
    FAST                    = 0x03,
    BALANCE                 = 0x04,
  };

  enum class CHARGING_MODE_NI : uint8_t {
    STANDARD                = 0x00,
    AUTO                    = 0x01,
    DISCHARGE               = 0x02,
    REPEAK                  = 0x03,
    CYCLE                   = 0x04,
  };

  enum class CHARGING_MODE_PB : uint8_t {
    CHARGE                  = 0x00,
    DISCHARGE               = 0x01,
  };

  enum class BATTERY_TYPE : uint8_t {
    LIPO                    = 0x00,
    LIIO                    = 0x01,
    LIFE                    = 0x02,
    LIHV                    = 0x03,
    NIMH                    = 0x04,
    NICD                    = 0x05,
    PB                      = 0x06,
  };

  enum class STATE : uint8_t {
    CHARGING                = 0x01,
    ERROR_1                 = 0x02,
    COMPLETE                = 0x03,
    ERROR_2                 = 0x04,
  };

  enum class ERROR : uint16_t {
    CONNECTION_BROKEN_1     = 0x000b,
    CELL_VOLTAGE_INVALID    = 0x000c,
    BALANCE_CONNECTION      = 0x000d,
    NO_BATTERY              = 0x000e,
    CELL_NUMBER_INCORRECT   = 0x000f,
    CONNECTION_MAIN_PORT    = 0x0010,
    BATTERY_FULL            = 0x0011,
    CHARGE_NOT_NEEDED       = 0x0012,
    CELL_HIGH_VOLTAGE       = 0x0013,
    CONNECTION_BROKEN_2     = 0x0014,
    CONNECTION_BROKEN_3     = 0x0015,
    CONNECTION_BROKEN_4     = 0x0016,
    INT_TEMP_TOO_HIGH       = 0x0100,
    EXT_TEMP_TOO_HIGH       = 0x0200,
    DC_IN_TOO_LOW           = 0x0300,
    DC_IN_TOO_HIGH          = 0x0400,
    OVER_TIME_LIMIT         = 0x0500,
    OVER_CAPACITY_LIMIT     = 0x0600,
    REVERSE_POLARITY        = 0x0700,

    CONTROL_FAIL            = 0x0800,
    BREAK_DOWN              = 0x0900,
    INPUT_FAIL              = 0x1000,

    UNKNOWN                 = 0xffff,
  };
}

#endif //LIB_B6_DEMO_ENUM_HH
