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
  class ChargingError : public std::exception {};

  class ErrorConnectionBroken : public ChargingError {};
  class ErrorCellVoltageInvalid : public ChargingError {};
  class ErrorBalanceConnection : public ChargingError {};
  class ErrorNoBattery : public ChargingError {};
  class ErrorCellNumberIncorrect : public ChargingError {};
  class ErrorConnectionMainPort : public ChargingError {};
  class ErrorBatteryFull : public ChargingError {};
  class ErrorChargeNotNeeded : public ChargingError {};
  class ErrorCellHighVoltage : public ChargingError {};
  class ErrorIntTempTooHigh : public ChargingError {};
  class ErrorExtTempTooHigh : public ChargingError {};
  class ErrorDCInTooLow : public ChargingError {};
  class ErrorDCInTooHigh : public ChargingError {};
  class ErrorOverTimeLimit : public ChargingError {};
  class ErrorOverCapacityLimit : public ChargingError {};
  class ErrorReversePolarity : public ChargingError {};
  // WTF LAND BELOW
  class ErrorControlFail : public ChargingError {};
  class ErrorBreakDown : public ChargingError {};
  class ErrorInputFail : public ChargingError {};
  class ErrorUnknown : public ChargingError {};
}

#endif //LIB_B6_DEMO_ERROR_HH
