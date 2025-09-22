#include "SensorUnitManager.h"
#include <esp_log.h>

static const char* SENSORUM_TAG = "sensor_unit_manager";

void SensorUnitManager::addUnit(const std::string& uuid)
{
    if (m_active_units.find(uuid) == m_active_units.end()) {
        m_active_units[uuid] = std::make_shared<std::string>(uuid);
        ESP_LOGI(SENSORUM_TAG, "Adding Sensor Unit %s", uuid.c_str());
    } else {
        ESP_LOGE(SENSORUM_TAG, "Sensor Unit %s already added", uuid.c_str());
    }
}

void SensorUnitManager::removeUnit(const std::string& uuid)
{
    auto it = m_active_units.find(uuid);
    if (it != m_active_units.end()) {
        m_active_units.erase(it);
        ESP_LOGI(SENSORUM_TAG, "Removed Sensor Unit %s", uuid.c_str());
    } else {
        ESP_LOGE(SENSORUM_TAG, "No Sensor Unit %s in list", uuid.c_str());
    }
}

bool SensorUnitManager::hasUnit(const std::string& uuid) const
{
    return (m_active_units.find(uuid) != m_active_units.end());
}

void SensorUnitManager::storeReading(const ca_sensorunit_snapshot& reading)
{
    m_all_readings.push_back(reading);
}

std::map<time_t, std::vector<ca_sensorunit_snapshot>> SensorUnitManager::getGroupedReadings() const
{
    std::map<time_t, std::vector<ca_sensorunit_snapshot>> grouped;
    for (const auto& snapshot : m_all_readings) {
        grouped[snapshot.timestamp].push_back(snapshot);
    }
    return grouped;
}

void SensorUnitManager::clearReadings()
{
    m_all_readings.clear();
}

    
