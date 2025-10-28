/**
 * @file JsonParser.h
 * @brief Static utility class for parsing and composing JSON related to sensor
 * and connection data.
 *
 * Provides functions for converting to and from internal data structures and
 * JSON strings From Sensor Unit: connect requests/responses, batched sensor
 * readings, From Backend: connect requests To Backend: batched sensor readings,
 * connect responses, error messages
 *
 * All methods are static and stateless, defined in the JsonParser class.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#pragma once
#include "connection_data_types.h"
#include "sensor_data_types.h"
#include <map>
#include <string>
#include <vector>

/**
 * @class JsonParser
 * @brief Static class for handling JSON serialization and deserialization.
 *
 * Converts sensor snapshots and connection-related data to and from JSON
 * format. Designed for use in communication between embedded devices and
 * backend services.
 *
 * All methods are static and stateless.
 */
class JsonParser {
  public:
    static std::string composeStatusRequest(const std::string& controlUnitId);
  
    static std::vector<SensorConnectRequest> parseStatusResponse(const std::string& json);

    /**
     * @brief Parses a JSON string containing grouped sensor snapshots.
     * @param json JSON-formatted string representing sensor readings.
     * @return Vector of parsed sensor snapshots.
     */
    static std::vector<ca_sensorunit_snapshot>
    parseSensorSnapshotGroup(const std::string& json);

    /**
     * @brief Composes a JSON string from grouped sensor readings.
     * @param readings Map of timestamp to vector of sensor snapshots.
     * @param controlunit_uuid UUID of the control unit sending the data.
     * @return JSON-formatted string representing the grouped readings.
     */
    static std::string composeGroupedReadings(
        const std::map<time_t, std::vector<ca_sensorunit_snapshot>>& readings,
        const std::string& controlunit_uuid);

    /**
     * @brief Parses a sensor connection request from JSON.
     * @param json JSON-formatted string representing the request.
     * @param type Type of request (e.g., connect, disconnect).
     * @return Parsed SensorConnectRequest object.
     */
    static SensorConnectRequest
    parseSensorConnectRequest(const std::string& json, requestType type);

    /**
     * @brief Composes a JSON response for a sensor connection.
     * @param response Response data to be sent.
     * @param controlunit_uuid UUID of the control unit.
     * @return JSON-formatted string representing the response.
     */
    static std::string
    composeSensorConnectResponse(const SensorConnectResponse& response,
                                 const std::string&           controlunit_uuid);

    /**
     * @brief Parses a Connect Request from a Sensor Unit
     *
     * @param json - Contains key "sensor_unit_id"
     * @return std::string - The sensor_unit_id as a string
     */
    static Uuid parseSensorunitConnectRequest(const std::string& json);

    /**
     * @brief Composes a Status Payload to a Sensor Unit
     *
     * @param status A string with the status:
     * Valid status: connected, pending, unvalid
     * @return std::string JSON payload to send to Sensor Unit
     */
    static std::string
    composeSensorunitStatusPayload(const std::string& status);

    /**
     * @brief Composes a JSON payload containing a Unix timestamp.
     *
     * Example: {"timestamp": <Unixtimestamp> }
     *
     * @param now The current time as a Unix timestamp (time_t).
     * @return A JSON-formatted string containing the timestamp.
     */
    static std::string composeTimestampPayload(time_t now);

    /**
     * @brief Composes a generic error response in JSON format. (Unused)
     * @param message Error message to include.
     * @param controlunit_uuid UUID of the control unit.
     * @return JSON-formatted string representing the error response.
     */
    static std::string
    composeErrorResponse(const std::string& message,
                         const std::string& controlunit_uuid);
};