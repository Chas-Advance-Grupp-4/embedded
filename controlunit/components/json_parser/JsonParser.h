/**
 * @file JsonParser.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Class for parsing and composing JSON to and from sensor data
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once
#include "connection_data_types.h"
#include "sensor_data_types.h"
#include <map>
#include <string>
#include <vector>

class JsonParser {
  public:
    /// Sensor readings
    static std::vector<ca_sensorunit_snapshot>
                       parseSensorSnapshotGroup(const std::string& json);
    static std::string composeGroupedReadings(
        const std::map<time_t, std::vector<ca_sensorunit_snapshot>>& readings,
        const std::string& controlunit_uuid);

    /// Sensor connection
    static SensorConnectRequest
    parseSensorConnectRequest(const std::string& json, requestType type);
    static std::string
    composeSensorConnectResponse(const SensorConnectResponse& response,
                                 const std::string&           controlunit_uuid);

    /// Driver connection
    static DriverConnectRequest
    parseDriverConnectRequest(const std::string& json, requestType type);
    static std::string
    composeDriverConnectResponse(const DriverConnectResponse& response,
                                 const std::string&           controlunit_uuid);

    /// Generic error/status
    static std::string
    composeErrorResponse(const std::string& message,
                         const std::string& controlunit_uuid);
};