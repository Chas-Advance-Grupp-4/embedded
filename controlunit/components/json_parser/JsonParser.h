#pragma once
#include "sensor_data_types.h"
#include <string>
#include <vector>
#include <map>

class JsonParser {
public:
    // Sensor readings
    static std::vector<ca_sensorunit_snapshot> parseSensorSnapshotGroup(const std::string& json);
    static std::string composeGroupedReadings(const std::map<time_t, std::vector<ca_sensorunit_snapshot>>& readings);

    // Sensor connection
    // static SensorConnectRequest parseSensorConnectRequest(const std::string& json);
    // static std::string composeSensorConnectResponse(const SensorConnectResponse& response);
    // static SensorDisconnectRequest parseSensorDisconnectRequest(const std::string& json);
    // static std::string composeSensorDisconnectResponse(const SensorDisconnectResponse& response);

    // Driver connection
    // static DriverConnectRequest parseDriverConnectRequest(const std::string& json);
    // static std::string composeDriverConnectResponse(const DriverConnectResponse& response);
    // static DriverDisconnectRequest parseDriverDisconnectRequest(const std::string& json);
    // static std::string composeDriverDisconnectResponse(const DriverDisconnectResponse& response);

    // Generic error/status
    // static std::string composeError(const std::string& message);
};