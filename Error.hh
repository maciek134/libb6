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

#ifndef LIB_B6_DEMO_ERROR_HH
#define LIB_B6_DEMO_ERROR_HH

#include <exception>

namespace b6 {
  class ChargingError : public std::exception {
  public:
    virtual std::string message() { return "Charging Error"; };
  };

  class ErrorConnectionBroken : public ChargingError {
    std::string message() { return "Connection broken!"; };
  };
  class ErrorCellVoltageInvalid : public ChargingError {
    std::string message() { return "Cell voltage invalid!"; };
  };
  class ErrorBalanceConnection : public ChargingError {
    std::string message() { return "Balance connection error!"; };
  };
  class ErrorNoBattery : public ChargingError {
    std::string message() { return"No battery detected!"; };
  };
  class ErrorCellNumberIncorrect : public ChargingError {
    std::string message() { return"Cell number incorrect!"; };
  };
  class ErrorConnectionMainPort : public ChargingError {
    std::string message() { return"Main battery port disconnected!"; };
  };
  class ErrorBatteryFull : public ChargingError {
    std::string message() { return"Battery full!"; };
  };
  class ErrorChargeNotNeeded : public ChargingError {
    std::string message() { return"Charging not needed!"; };
  };
  class ErrorCellHighVoltage : public ChargingError {
    std::string message() { return"Cell voltage high!"; };
  };
  class ErrorIntTempTooHigh : public ChargingError {
    std::string message() { return"Internal temperature too high!"; };
  };
  class ErrorExtTempTooHigh : public ChargingError {
    std::string message() { return"External temperature too high!"; };
  };
  class ErrorDCInTooLow : public ChargingError {
    std::string message() { return"DC-in too low!"; };
  };
  class ErrorDCInTooHigh : public ChargingError {
    std::string message() { return"DC-in too high!"; };
  };
  class ErrorOverTimeLimit : public ChargingError {
    std::string message() { return"Time limit reached!"; };
  };
  class ErrorOverCapacityLimit : public ChargingError {
    std::string message() { return"Capacity limit reached!"; };
  };
  class ErrorReversePolarity : public ChargingError {
    std::string message() { return"Reverse polarity!"; };
  };
  // WTF LAND BELOW
  class ErrorControlFail : public ChargingError {
    std::string message() { return"WTF: Control Fail"; };
  };
  class ErrorBreakDown : public ChargingError {
    std::string message() { return"WTF: Break Down"; };
  };
  class ErrorInputFail : public ChargingError {
    std::string message() { return"WTF: Input Fail"; };
  };
  class ErrorUnknown : public ChargingError {
    std::string message() { return"WTF: Unknown"; };
  };
}

#endif //LIB_B6_DEMO_ERROR_HH
