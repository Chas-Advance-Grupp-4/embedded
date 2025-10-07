/**
 * @file connection_data_types.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implementation of helper functions for sensor data communication
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#include "connection_data_types.h"

/**
 * @brief Return a string corresponding to a requestType
 * 
 * @param request 
 * @return std::string 
 */
std::string requestTypeToString(requestType request){
    switch (request) {
        case requestType::CONNECT:    return "connect";
        case requestType::DISCONNECT:    return "disconnect";
        default:                             return "unknown";
    }
}

/**
 * @brief Return a string corresponding to a connectionStatus
 * 
 * @param status 
 * @return std::string 
 */
std::string connectionStatusToString(connectionStatus status) {
    switch (status) {
        case connectionStatus::CONNECTED:    return "connected";
        case connectionStatus::DISCONNECTED: return "disconnected";
        case connectionStatus::PENDING:      return "pending";
        case connectionStatus::UNAVAILABLE:  return "unavailable";
        default:                             return "unknown";
    }
}
