/**
 * @file JsonParser.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Declaration of the JsonParser class for serializing sensor readings into JSON format.
 *
 * This class provides static methods to convert a collection of sensor readings
 * into a structured JSON string suitable for transmission or storage.
 * @date 2025-10-08
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "communication_data_types.h"
#include "constants.h"
#include "sensor_data_types.h"
#include <ArduinoJson.h>
#include <etl/string.h>
#include <etl/vector.h>

/**
 * @class JsonParser
 * @brief Static class for handling JSON serialization and deserialization.
 *
 *
 * All methods are static and stateless.
 */
class JsonParser {
  public:
    /**
     * @brief Composes a JSON string representing a group of sensor readings.
     *
     * @param readings
     * @param uuid
     * @return etl::string<MAX_JSON_SIZE> A JSON string
     */
    static etl::string<json_config::max_json_size> composeSensorSnapshotGroup(
        etl::vector<CaSensorunitReading, json_config::max_batch_size>& readings,
        const char*                                                    uuid);
    /**
     * @brief Composes a JSON string for connecting with Control Unit
     *
     * @param su_uuid This Sensor Unit UUID
     * @return etl::string<json_config::max_small_json_size> a JSON string
     * @example return string: {"sensor_unit_id":"550e8400-e29b-41d4-a716-446655440000"}
     */
    static etl::string<json_config::max_small_json_size> composeConnectRequest(const char* su_uuid);
    /**
     * @brief parse a JSON connect response
     *
     * @param payload The JSON payload received from the Control Unit
     * @return ConnectResponse Struct containing connection status and assigned sensor ID
     *
     * @example:
     *   {
     *     connected = true,
     *     sensorId = 1
     *   }
     */
    static ConnectResponse
    parseConnectResponse(etl::string<json_config::max_small_json_size> payload);
    /**
     * @brief Parse a JSON response from GET /time
     *
     * @param payload The JSON payload received from the Control Unit
     * @return unsigned long - Unix timestamp with current time
     */
    static unsigned long
    parseGetTimeResponse(etl::string<json_config::max_small_json_size> payload);

  private:
    static constexpr const char* TAG = "JsonParser";
};
