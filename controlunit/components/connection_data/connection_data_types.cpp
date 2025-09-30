#include "connection_data_types.h"

std::string connectionStatusToString(connectionStatus status) {
    switch (status) {
        case connectionStatus::CONNECTED:    return "connected";
        case connectionStatus::DISCONNECTED: return "disconnected";
        case connectionStatus::PENDING:      return "pending";
        case connectionStatus::UNAVAILABLE:  return "unavailable";
        default:                             return "unknown";
    }
}
