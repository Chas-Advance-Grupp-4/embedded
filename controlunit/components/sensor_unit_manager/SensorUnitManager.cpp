/**
 * @file SensorUnitManager.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Classes that handle and store readings from Sensor Units
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#include "SensorUnitManager.h"
#include <esp_log.h>

static const char* SENSORUM_TAG = "sensor_unit_manager";

/**
 * @brief Add a sensorunit to the Manager
 * 
 * @param uuid of the sensor unit
 */
void SensorUnitManager::addUnit(const Uuid& uuid) {
    if (m_active_units.find(uuid) == m_active_units.end()) {
        m_active_units[uuid] = std::make_shared<Uuid>(uuid);
        ESP_LOGI(SENSORUM_TAG, "Adding Sensor Unit %s", uuid.toString().c_str());
    } else {
        ESP_LOGE(SENSORUM_TAG, "Sensor Unit %s already added", uuid.toString().c_str());
    }
}

/**
 * @brief Remove a sensorunit from the Manager
 * 
 * @param uuid of the sensor unit to remove
 */
void SensorUnitManager::removeUnit(const Uuid& uuid) {
    auto it = m_active_units.find(uuid);
    if (it != m_active_units.end()) {
        m_active_units.erase(it);
        ESP_LOGI(SENSORUM_TAG, "Removed Sensor Unit %s", uuid.toString().c_str());
    } else {
        ESP_LOGE(SENSORUM_TAG, "No Sensor Unit %s in list", uuid.toString().c_str());
    }
}

/**
 * @brief Check if a sensorunit is added to the Manager
 * 
 * @param uuid 
 * @return true 
 * @return false 
 */
bool SensorUnitManager::hasUnit(const Uuid& uuid) const {
    return (m_active_units.find(uuid) != m_active_units.end());
}

/**
 * @brief Store an individual reading from a sensor unit
 * 
 * @param reading type defined in sensor_data_types.h
 */
void SensorUnitManager::storeReading(const ca_sensorunit_snapshot& reading) {
    m_all_readings.push_back(reading);
}

/**
 * @brief Group all readings on timestamp for batch sending to backend
 * 
 * @return std::map<time_t, std::vector<ca_sensorunit_snapshot>> 
 */
std::map<time_t, std::vector<ca_sensorunit_snapshot>>
SensorUnitManager::getGroupedReadings() const {
    std::map<time_t, std::vector<ca_sensorunit_snapshot>> grouped;
    for (const auto& snapshot : m_all_readings) {
        grouped[snapshot.timestamp].push_back(snapshot);
    }
    return grouped;
}

/**
 * @brief Clear all readings
 * 
 */
void SensorUnitManager::clearReadings() {
    m_all_readings.clear();
}
