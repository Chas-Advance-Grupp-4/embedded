/**
 * @file ControlUnitManager.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Class for handling all internal state for Contol Unit
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#include "ControlUnitManager.h"
#include "esp_log.h"

/**
 * @brief Constructor for Control Unit Manager
 *
 * @param controlunitUuid
 */
ControlUnitManager::ControlUnitManager(SensorUnitManager& sensorUnitManager,
                                       const std::string& controlunitUuid)
    : sensorManager{sensorUnitManager}, m_controlunitUuid{controlunitUuid} {}

/**
 * @brief Query if a driver is connected
 *
 * @return true
 * @return false
 */
bool ControlUnitManager::isDriverConnected() {
    return m_isDriverConnected;
}

/**
 * @brief Connect a driver to Control Unit
 *
 * TODO: implement
 */
void ControlUnitManager::connectDriver() {}

/**
 * @brief Disconnect a driver to Control Unit
 *
 * TODO: implement
 */
void ControlUnitManager::disconnectDriver() {}

/**
 * @brief Get connected driver_id
 *
 * TODO: Refactor to use UUID
 *
 * @return uint32_t
 */
uint32_t ControlUnitManager::getDriverId() {
    if (m_isDriverConnected) {
        return m_driverId;
    } else {
        return 0;
    }
}

/**
 * @brief Get the Control Unit UUID as a string
 *
 * @return std::string
 */
std::string ControlUnitManager::getControlunitUuidString() {
    if (m_controlunitUuid.isValid())
        return m_controlunitUuid.toString();
    else {
        ESP_LOGE(TAG, "No valid Controlunit Uuid. Returning blank string");
        return "";
    }
}