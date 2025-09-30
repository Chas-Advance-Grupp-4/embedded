#pragma once
#include "sensor_data_types.h"

enum class requestType { CONNECT, DISCONNECT };

std::string requestTypeToString(requestType request);

struct SensorConnectRequest {
    std::shared_ptr<Uuid> sensorUuid;
    requestType           request;
    std::string           token;
};

struct DriverConnectRequest {
    uint32_t    driverId {};
    requestType request;
    std::string token;
};

enum class connectionStatus {
    CONNECTED,
    DISCONNECTED,
    PENDING,
    UNAVAILABLE,
};

std::string connectionStatusToString(connectionStatus status);

struct SensorConnectResponse {
    std::shared_ptr<Uuid> sensorUuid;
    connectionStatus      status;
};

struct DriverConnectResponse {
    uint32_t         driverId;
    connectionStatus status;
};