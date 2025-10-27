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

void SensorUnitManager::init() {
    ESP_LOGI(TAG, "Initializing Sensor Unit Manager");
    m_readingsMutex = xSemaphoreCreateMutex();
    if (m_readingsMutex == nullptr) {
        ESP_LOGE(TAG, "Failed to create mutex");
    }
}

void SensorUnitManager::addUnit(const Uuid& uuid) {
    if (m_active_units.find(uuid) == m_active_units.end()) {
        m_active_units[uuid] = std::make_shared<Uuid>(uuid);
        ESP_LOGI(TAG, "Adding Sensor Unit %s", uuid.toString().c_str());
    } else {
        ESP_LOGE(TAG, "Sensor Unit %s already added", uuid.toString().c_str());
    }
}

void SensorUnitManager::removeUnit(const Uuid& uuid) {
    auto it = m_active_units.find(uuid);
    if (it != m_active_units.end()) {
        m_active_units.erase(it);
        ESP_LOGI(TAG, "Removed Sensor Unit %s", uuid.toString().c_str());
    } else {
        ESP_LOGE(TAG, "No Sensor Unit %s in list", uuid.toString().c_str());
    }
}

bool SensorUnitManager::hasUnit(const Uuid& uuid) const {
    return (m_active_units.find(uuid) != m_active_units.end());
}

void SensorUnitManager::storeReading(const ca_sensorunit_snapshot& reading) {
    ESP_LOGI(TAG, "Storing reading, mutex protected");
    if (xSemaphoreTake(m_readingsMutex, portMAX_DELAY) == pdTRUE) {
        m_all_readings.push_back(reading);
        xSemaphoreGive(m_readingsMutex);
    }
}

std::map<time_t, std::vector<ca_sensorunit_snapshot>>
SensorUnitManager::getGroupedReadings() const {
    ESP_LOGI(TAG, "Getting grouped readings, mutex protected");
    std::map<time_t, std::vector<ca_sensorunit_snapshot>> grouped;
    // Consider copying vector to avoid blocking mutex
    if (xSemaphoreTake(m_readingsMutex, portMAX_DELAY) == pdTRUE) {
        for (const auto& snapshot : m_all_readings) {
            grouped[snapshot.timestamp].push_back(snapshot);
        }
        xSemaphoreGive(m_readingsMutex);
    }
    return grouped;
}

void SensorUnitManager::clearReadings() {
    ESP_LOGI(TAG, "Clearing readings, mutex protected");
    if (xSemaphoreTake(m_readingsMutex, portMAX_DELAY) == pdTRUE) {
        m_all_readings.clear();
        xSemaphoreGive(m_readingsMutex);
    }
}
