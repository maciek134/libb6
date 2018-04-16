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

#ifndef LIB_B6_DEMO_B6DEVICE_HH
#define LIB_B6_DEMO_B6DEVICE_HH

#include <string>
#include <vector>
#include <libusb.h>
#include <stdexcept>
#include "Packet.hh"
#include "Enum.hh"
#include "Error.hh"

#define B6_VENDOR_ID 0x0000
#define B6_PRODUCT_ID 0x0001

namespace b6 {
  struct SysInfo {
    unsigned int cycleTime, timeLimit, capLimit, lowDCLimit, tempLimit, voltage, cells[8];
    bool timeLimitOn, capLimitOn, keyBuzzer, systemBuzzer;
  };

  struct ChargeInfo {
    unsigned int state, tempExt, tempInt, capacity, time, voltage, current, impendance, cells[8];
  };

  struct ChargeProfile {
    BATTERY_TYPE batteryType;
    uint8_t cellCount, rPeakCount, cycleType, cycleCount;
    union {
      CHARGING_MODE_LI li;
      CHARGING_MODE_NI ni;
      CHARGING_MODE_PB pb;
    } mode;
    uint16_t chargeCurrent, dischargeCurrent, cellDischargeVoltage, endVoltage, trickleCurrent;
  };

  class Device {
  public:
    Device();
    ~Device();
    SysInfo getSysInfo();
    ChargeInfo getChargeInfo();

    std::string getCoreType() { return m_coreType; };
    int getUpgradeType() { return m_upgradeType; };
    int getLanguageID() { return m_languageID; };
    int getCustomerID() { return m_customerID; };
    double getHWVersion() { return m_hwVersion; };
    double getSWVersion() { return m_swVersion; };
    bool isEncrypted() { return m_isEncrypted; };
    int getCellCount() { return m_cellCount; };
    ChargeProfile getDefaultChargeProfile(BATTERY_TYPE type);

    bool setCycleTime(int cycleTime);
    bool setTimeLimit(bool enabled, int limit);
    bool setCapacityLimit(bool enabled, int limit);
    bool setTempLimit(int limit);
    bool setBuzzers(bool system, bool key);

    void stopCharging();
    bool startCharging(ChargeProfile profile);

    static bool isBatteryLi(BATTERY_TYPE type) { return type >= BATTERY_TYPE::LIPO && type <= BATTERY_TYPE::LIHV; };
    static bool isBatteryNi(BATTERY_TYPE type) { return type == BATTERY_TYPE::NIMH || type == BATTERY_TYPE::NICD; };
  private:
    libusb_context *m_libusbCtx{};
    libusb_device_handle *m_dev;
    bool m_hadKernelDriver = false;

    int m_cellCount = 6;
    std::string m_coreType;
    int m_upgradeType{}, m_languageID{}, m_customerID{};
    double m_hwVersion{}, m_swVersion{};
    bool m_isEncrypted{};

    Packet m_read();
    void m_write(Packet packet);

    Packet m_sendCommand(CMD cmd);

    void m_getDevInfo();

    ChargingError m_throwError(ERROR err);
  };
}

#endif //LIB_B6_DEMO_B6DEVICE_HH
