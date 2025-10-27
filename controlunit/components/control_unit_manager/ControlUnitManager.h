/**
 * @file ControlUnitManager.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Class for handling all internal state for Contol Unit
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "SensorUnitManager.h"

class ControlUnitManager {
  public:
    ControlUnitManager(SensorUnitManager& sensorUnitManager,
                       const std::string& controlunitUuid =
                           "f47ac10b-58cc-4372-a567-0e02b2c3d479");
    SensorUnitManager& sensorManager;
    std::string        getControlunitUuidString();

  private:
    Uuid                         m_controlunitUuid;
    static constexpr const char* TAG = "ControlUnitManager";
};