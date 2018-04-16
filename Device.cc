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

#include "Device.hh"

namespace b6 {
  Device::Device() {
    int err = libusb_init(&m_libusbCtx);
    if (err != 0) {
      throw std::runtime_error("libusb err: " + std::to_string(err));
    }
    m_dev = libusb_open_device_with_vid_pid(m_libusbCtx, B6_VENDOR_ID, B6_PRODUCT_ID);
    if (m_dev == nullptr) {
      throw std::runtime_error("cannot find/open b6 device");
    }

    if (libusb_kernel_driver_active(m_dev, 0) == 1) {
      m_hadKernelDriver = true;
      err = libusb_detach_kernel_driver(m_dev, 0);
      if (err != 0) {
        throw std::runtime_error("cannot detach kernel driver, err: " + std::to_string(err));
      }
    }
    err = libusb_claim_interface(m_dev, 0);
    if (err != 0) {
      throw std::runtime_error("cannot claim interface 0, err: " + std::to_string(err));
    }

    m_getDevInfo();
  }

  Device::~Device() {
    if (m_dev != nullptr) {
      libusb_release_interface(m_dev, 0);
      if (m_hadKernelDriver) {
        libusb_attach_kernel_driver(m_dev, 0);
      }
      libusb_close(m_dev);
    }
    libusb_exit(m_libusbCtx);
  }

  SysInfo Device::getSysInfo() {
    Packet res = m_sendCommand(CMD::GET_SYS_INFO);
    res.skip(4);

    SysInfo info = {0};
    info.cycleTime = res.readU8();
    info.timeLimitOn = res.readU8();
    info.timeLimit = res.readU16();
    info.capLimitOn = res.readU8();
    info.capLimit = res.readU16();
    info.keyBuzzer = res.readU8();
    info.systemBuzzer = res.readU8();
    info.lowDCLimit = res.readU16();
    res.skip(2);
    info.tempLimit = res.readU8();
    info.voltage = res.readU16();

    for (unsigned int &cell : info.cells) {
      cell = res.readU16();
    }

    return info;
  }

  ChargeInfo Device::getChargeInfo() {
    ChargeInfo info = {0};

    Packet res = m_sendCommand(CMD::GET_CHARGE_INFO);
    res.skip(4);

    info.state = res.readU8(); // TODO: finish enum STATE and convert this

    if (info.state == static_cast<int>(STATE::ERROR_1) || info.state == static_cast<int>(STATE::ERROR_2)) {
      m_throwError(static_cast<ERROR>(res.readU16()));
    }

    info.capacity = res.readU16();
    info.time = res.readU16();
    info.voltage = res.readU16();
    info.current = res.readU16();
    info.tempExt = res.readU8();
    info.tempInt = res.readU8();
    info.impendance = res.readU16();

    for (int i = 0; i < m_cellCount; i++) {
      info.cells[i] = res.readU16();
    }

    return info;
  }

  Packet Device::m_read() {
    std::vector<uint8_t> buf(64);
    int len = 0;
    libusb_interrupt_transfer(m_dev, 0x81, &buf[0], 64, &len, 200);
    return Packet(buf);
  }

  void Device::m_write(Packet packet) {
    int len = 0;
    libusb_interrupt_transfer(m_dev, 0x01, packet.getBuffer(), packet.getSize(), &len, 200);
  }

  Packet Device::m_sendCommand(CMD cmd) {
    Packet cmdPacket({ 0x0f, 0x03, static_cast<uint8_t>(cmd), 0x00 });
    cmdPacket.writeChecksum();
    m_write(cmdPacket);
    Packet res = m_read();
    return res;
  }

  void Device::m_getDevInfo() {
    Packet res = m_sendCommand(CMD::GET_DEV_INFO);
    res.skip(5);

    m_coreType = res.readString(6);
    m_upgradeType = res.readU8();
    m_isEncrypted = res.readU8();
    m_customerID = res.readU16();
    m_languageID = res.readU8();
    m_swVersion = ((double)res.readU8() + (double)res.readU8()) / 100.0;
    m_hwVersion = (double)res.readU8();

    if (m_coreType == "100069") {
      m_cellCount = 8;
    } else {
      m_cellCount = 6;
    }
  }

  ChargingError Device::m_throwError(ERROR err) {
    switch (err) {
      case ERROR::CONNECTION_BROKEN_1:
      case ERROR::CONNECTION_BROKEN_2:
      case ERROR::CONNECTION_BROKEN_3:
      case ERROR::CONNECTION_BROKEN_4:    throw ErrorConnectionBroken();
      case ERROR::CELL_VOLTAGE_INVALID:   throw ErrorCellVoltageInvalid();
      case ERROR::BALANCE_CONNECTION:     throw ErrorBalanceConnection();
      case ERROR::NO_BATTERY:             throw ErrorNoBattery();
      case ERROR::CELL_NUMBER_INCORRECT:  throw ErrorCellNumberIncorrect();
      case ERROR::CONNECTION_MAIN_PORT:   throw ErrorConnectionMainPort();
      case ERROR::BATTERY_FULL:           throw ErrorBatteryFull();
      case ERROR::CHARGE_NOT_NEEDED:      throw ErrorChargeNotNeeded();
      case ERROR::CELL_HIGH_VOLTAGE:      throw ErrorCellHighVoltage();
      case ERROR::INT_TEMP_TOO_HIGH:      throw ErrorIntTempTooHigh();
      case ERROR::EXT_TEMP_TOO_HIGH:      throw ErrorExtTempTooHigh();
      case ERROR::DC_IN_TOO_LOW:          throw ErrorDCInTooLow();
      case ERROR::DC_IN_TOO_HIGH:         throw ErrorDCInTooHigh();
      case ERROR::OVER_TIME_LIMIT:        throw ErrorOverTimeLimit();
      case ERROR::OVER_CAPACITY_LIMIT:    throw ErrorOverCapacityLimit();
      case ERROR::REVERSE_POLARITY:       throw ErrorReversePolarity();
      case ERROR::CONTROL_FAIL:           throw ErrorControlFail();
      case ERROR::BREAK_DOWN:             throw ErrorBreakDown();
      case ERROR::INPUT_FAIL:             throw ErrorInputFail();
      case ERROR::UNKNOWN:                throw ErrorUnknown();
    }
  }

  bool Device::setCycleTime(int cycleTime) {
    if (cycleTime < 1 || cycleTime > 60) {
      return false;
    }
    Packet cmd({ 0x0f, 0x05, 0x11, 0x00, 0x00 });
    cmd.writeU8(static_cast<uint8_t>(cycleTime));
    cmd.writeChecksum();

    m_write(cmd);
    m_read(); // TODO: handle return value
    return true;
  }

  bool Device::setTimeLimit(bool enabled, int limit) {
    if (limit < 1 || limit > 720) {
      return false;
    }
    Packet cmd({ 0x0f, 0x07, 0x11, 0x01, 0x00 });
    cmd.writeU8(static_cast<uint8_t>(enabled));
    cmd.writeU16(static_cast<uint16_t>(limit));
    cmd.writeChecksum();

    m_write(cmd);
    m_read(); // TODO: handle return value
    return true;
  }

  bool Device::setCapacityLimit(bool enabled, int limit) {
    if (limit < 100 || limit > 50000) {
      return false;
    }
    Packet cmd({ 0x0f, 0x07, 0x11, 0x02, 0x00 });
    cmd.writeU8(static_cast<uint8_t>(enabled));
    cmd.writeU16(static_cast<uint16_t>(limit));
    cmd.writeChecksum();

    m_write(cmd);
    m_read(); // TODO: handle return value
    return true;
  }

  bool Device::setTempLimit(int limit) {
    if (limit < 20 || limit > 80) {
      return false;
    }
    Packet cmd({ 0x0f, 0x05, 0x11, 0x05, 0x00 });
    cmd.writeU8(static_cast<uint8_t>(limit));
    cmd.writeChecksum();

    m_write(cmd);
    m_read(); // TODO: handle return value
    return true;
  }

  bool Device::setBuzzers(bool system, bool key) {
    Packet cmd({ 0x0f, 0x06, 0x11, 0x03, 0x00 });
    cmd.writeU8(static_cast<uint8_t>(key));
    cmd.writeU8(static_cast<uint8_t>(system));
    cmd.writeChecksum();

    m_write(cmd);
    m_read(); // TODO: handle return value
    return true;
  }

  void Device::stopCharging() {
    m_sendCommand(CMD::STOP_CHARGING);
  }

  bool Device::startCharging(ChargeProfile profile) {
    Packet res = m_sendCommand(CMD::UNK1);
    if (res.readU8() == 4) {
      stopCharging();
    }

    Packet cmd({ 0x0f, 0x16, 0x05, 0x00 });
    cmd.writeU8(static_cast<uint8_t>(profile.batteryType));
    cmd.writeU8(profile.cellCount);
    if (isBatteryLi(profile.batteryType)) {
      cmd.writeU8(static_cast<uint8_t>(profile.mode.li));
    } else if (isBatteryNi(profile.batteryType)) {
      cmd.writeU8(static_cast<uint8_t>(profile.mode.ni));
    } else {
      cmd.writeU8(static_cast<uint8_t>(profile.mode.pb));
    }
    cmd.writeU16(profile.chargeCurrent);
    cmd.writeU16(profile.dischargeCurrent);
    cmd.writeU16(profile.cellDischargeVoltage);
    cmd.writeU16(profile.endVoltage);
    if (isBatteryNi(profile.batteryType) && profile.mode.ni >= CHARGING_MODE_NI::REPEAK) {
      if (profile.mode.ni == CHARGING_MODE_NI::REPEAK) {
        cmd.writeU8(profile.rPeakCount);
        cmd.writeU8(0x00);
      } else if (profile.mode.ni == CHARGING_MODE_NI::CYCLE) {
        cmd.writeU8(profile.cycleType);
        cmd.writeU8(profile.cycleCount);
      }
    } else {
      cmd.writeUA({ 0x00, 0x00 });
    }
    cmd.writeU16(profile.trickleCurrent);
    cmd.writeUA({ 0x00, 0x00, 0x00, 0x00 });
    cmd.writeChecksum();

    m_write(cmd);
    m_read(); // TODO: handle return value

    return true;
  }

  ChargeProfile Device::getDefaultChargeProfile(BATTERY_TYPE type) {
    ChargeProfile profile = {};
    profile.batteryType = type;
    profile.cellCount = 1;
    profile.rPeakCount = 3;
    profile.cycleType = 1;
    profile.cycleCount = 1;
    if (isBatteryLi(type)) {
      profile.mode.li = CHARGING_MODE_LI::STANDARD;
    } else if (isBatteryNi(type)) {
      profile.mode.ni = CHARGING_MODE_NI::STANDARD;
    } else {
      profile.mode.pb = CHARGING_MODE_PB::CHARGE;
    }
    profile.chargeCurrent = 1500;
    profile.dischargeCurrent = 1000;
    profile.trickleCurrent = 0;
    switch (type) {
      case BATTERY_TYPE::LIPO:
        profile.cellDischargeVoltage = 3200;
        profile.endVoltage = 4200;
        break;
      case BATTERY_TYPE::LIIO:
        profile.cellDischargeVoltage = 3100;
        profile.endVoltage = 4200;
        break;
      case BATTERY_TYPE::LIFE:
        profile.cellDischargeVoltage = 2900;
        profile.endVoltage = 3700;
        break;
      case BATTERY_TYPE::LIHV:
      case BATTERY_TYPE::NIMH:
        profile.cellDischargeVoltage = 1100;
        profile.endVoltage = 4;
        break;
      case BATTERY_TYPE::NICD:
        profile.cellDischargeVoltage = 1800;
        profile.endVoltage = 0;
        profile.trickleCurrent = 300;
        break;
      case BATTERY_TYPE::PB:
        profile.cellDischargeVoltage = 11000;
        profile.endVoltage = 12000;
        break;
    }

    return profile;
  }
}