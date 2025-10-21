/**
 * @file SensorUnitManager.cpp
 * @brief Implementation of SensorUnitManager for managing sensor units and their readings.
 *
 * Defines the logic for adding/removing units, storing readings, and grouping them
 * for backend dispatch. Uses ESP-IDF logging for diagnostics.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#include "SensorUnitManager.h"
#include <esp_log.h>

static const char* SENSORUM_TAG = "sensor_unit_manager";

void SensorUnitManager::addUnit(const Uuid& uuid) {
    if (m_active_units.find(uuid) == m_active_units.end()) {
        m_active_units[uuid] = std::make_shared<Uuid>(uuid);
        ESP_LOGI(SENSORUM_TAG, "Adding Sensor Unit %s", uuid.toString().c_str());
    } else {
        ESP_LOGE(SENSORUM_TAG, "Sensor Unit %s already added", uuid.toString().c_str());
    }
}

void SensorUnitManager::removeUnit(const Uuid& uuid) {
    auto it = m_active_units.find(uuid);
    if (it != m_active_units.end()) {
        m_active_units.erase(it);
        ESP_LOGI(SENSORUM_TAG, "Removed Sensor Unit %s", uuid.toString().c_str());
    } else {
        ESP_LOGE(SENSORUM_TAG, "No Sensor Unit %s in list", uuid.toString().c_str());
    }
}

bool SensorUnitManager::hasUnit(const Uuid& uuid) const {
    return (m_active_units.find(uuid) != m_active_units.end());
}

void SensorUnitManager::storeReading(const ca_sensorunit_snapshot& reading) {
    m_all_readings.push_back(reading);
}

std::map<time_t, std::vector<ca_sensorunit_snapshot>>
SensorUnitManager::getGroupedReadings() const {
    std::map<time_t, std::vector<ca_sensorunit_snapshot>> grouped;
    for (const auto& snapshot : m_all_readings) {
        grouped[snapshot.timestamp].push_back(snapshot);
    }
    return grouped;
}

void SensorUnitManager::clearReadings() {
    m_all_readings.clear();
}
