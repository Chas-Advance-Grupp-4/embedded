#pragma once
#include "sensor_data_types.h"

enum class requestType { CONNECT, DISCONNECT };

struct SensorConnectRequest {
    std::shared_ptr<Uuid> sensorUuid;
    requestType           request;
    std::string           token;
};

enum class connectionStatus {
    CONNECTED,
    DISCONNECTED,
    PENDING,
    UNAVAILABLE,
};

struct SensorConnectResponse {
    std::shared_ptr<Uuid> sensorUuid;
    connectionStatus      status;
};