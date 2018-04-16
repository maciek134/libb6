//
// Created by klh on 4/13/18.
//

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
