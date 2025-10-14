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
#include "constants.h"
#include "sensor_data_types.h"
#include "communication_data_types.h"
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
     * @brief Composes a JSON string with a connect response
     *
     * @param su_uuid
     * @return etl::string<json_config::max_small_json_size>
     */
    static etl::string<json_config::max_small_json_size> composeConnectRequest(const char* su_uuid);
    /**
     * @brief parse a JSON connect response
     *
     * @param payload
     * @return ConnectResponse
     */
    static ConnectResponse parseConnectResponse(etl::string<json_config::max_small_json_size> payload);
    /**
     * @brief Parse a JSON response from GET /time
     *
     * @param payload
     * @return unsigned long - Unix timestamp with current time
     */
    static unsigned long parseGetTimeResponse(etl::string<json_config::max_small_json_size> payload);
    private:
      static constexpr const char* TAG = "JsonParser";
};
