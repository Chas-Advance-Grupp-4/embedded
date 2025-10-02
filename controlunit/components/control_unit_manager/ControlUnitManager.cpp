#include "ControlUnitManager.h"
#include "esp_log.h"

ControlUnitManager::ControlUnitManager(const std::string& controlunitUuid)
    : m_controlunitUuid{controlunitUuid} {}

bool ControlUnitManager::isDriverConnected() {
    return m_isDriverConnected;
}
void ControlUnitManager::connectDriver() {
    // TODO: implement
}
void ControlUnitManager::disconnectDriver() {
    // TODO: implement
}
uint32_t ControlUnitManager::getDriverId() {
    if (m_isDriverConnected) {
        return m_driverId;
    } else {
        return 0;
    }
}

std::string ControlUnitManager::getControlunitUuidString() {
    if (m_controlunitUuid.isValid())
        return m_controlunitUuid.toString();
    else {
        ESP_LOGE(TAG, "No valid Controlunit Uuid. Returning blank string");
        return "";
    }
}