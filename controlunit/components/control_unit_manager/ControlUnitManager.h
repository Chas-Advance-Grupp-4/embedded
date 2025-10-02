#pragma once
#include "SensorUnitManager.h"

class ControlUnitManager {
  public:
    ControlUnitManager(const std::string& controlunitUuid = "f47ac10b-58cc-4372-a567-0e02b2c3d479");
    SensorUnitManager sensorManager;
    bool              isDriverConnected();
    void              connectDriver();
    void              disconnectDriver();
    uint32_t          getDriverId();
    std::string       getControlunitUuidString();

  private:
    bool     m_isDriverConnected { false };
    uint32_t m_driverId {};
    Uuid     m_controlunitUuid;
    static constexpr const char* TAG = "ControlUnitManager";
};