#pragma once
#include "sensor_data_types.h"
#include <map>
#include <string>
#include <vector>

class SensorUnitManager {
  public:
    void addUnit(const Uuid& uuid);
    void removeUnit(const Uuid& uuid);
    bool hasUnit(const Uuid& uuid) const;

    void storeReading(const ca_sensorunit_snapshot& reading);
    std::map<time_t, std::vector<ca_sensorunit_snapshot>> getGroupedReadings() const;
    void                                                  clearReadings();

  private:
    std::map<Uuid, std::shared_ptr<Uuid>> m_active_units;
    std::vector<ca_sensorunit_snapshot>   m_all_readings;
};