#pragma once
#include "sensor_data_types.h"
#include <string>
#include <vector>
#include <map>

class SensorUnitManager {
public:
    void addUnit(const std::string& uuid);
    void removeUnit(const std::string& uuid);
    bool hasUnit(const std::string& uuid) const;

    void storeReading(const ca_sensorunit_snapshot& reading);
    std::map<time_t, std::vector<ca_sensorunit_snapshot>> getGroupedReadings() const;
    void clearReadings();

private:
    std::map<std::string, std::shared_ptr<std::string>> m_active_units;
    std::vector<ca_sensorunit_snapshot> m_all_readings;
};