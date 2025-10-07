/**
 * @file connection_data_types.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Structs and enums for handling communication between Control Unit,
 * Backend and Sensor Units
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once
#include "sensor_data_types.h"

/**
 * @brief Valid request for connecting a Driver as well as Sensor Unit
 * 
 */
enum class requestType { CONNECT, DISCONNECT };

/**
 * @brief Helper function to convert a requestType to a string
 * 
 * @param request 
 * @return std::string 
 */
std::string requestTypeToString(requestType request);

/**
 * @brief Struct used for composing a Sensor Connect Request
 * 
 */
struct SensorConnectRequest {
    std::shared_ptr<Uuid> sensorUuid;
    requestType           request;
    std::string           token;
};

/**
 * @brief Struct used for composing a Driver Connect Request
 * 
 */
struct DriverConnectRequest {
    uint32_t    driverId {};
    requestType request;
    std::string token;
};

/**
 * @brief Valid status for communicating connection status
 * for both Sensor Unit and Driver
 * 
 */
enum class connectionStatus {
    CONNECTED,
    DISCONNECTED,
    PENDING,
    UNAVAILABLE,
};

/**
 * @brief Helper function to convert a connectionStatus to a string
 * 
 * @param status 
 * @return std::string 
 */
std::string connectionStatusToString(connectionStatus status);

/**
 * @brief Struct used for composing a Sensor Unit Connect Response
 * 
 */
struct SensorConnectResponse {
    std::shared_ptr<Uuid> sensorUuid;
    connectionStatus      status;
};

/**
 * @brief Struct used for composing a Driver Connect Response
 * 
 */
struct DriverConnectResponse {
    uint32_t         driverId;
    connectionStatus status;
};