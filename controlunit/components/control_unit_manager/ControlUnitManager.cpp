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