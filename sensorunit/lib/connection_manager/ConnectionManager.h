#pragma once
#include "RestClient.h"
#include "communication_data_types.h"
#include "config.h"
#include <Arduino.h>
#include <etl/string.h>
#include <etl/vector.h>

namespace connection_types {
struct fw_version {
    int major;
    int minor;
    int patch;
};

struct cu_candidate {
    etl::string<32> ssid;
    int             rssi;
};

}; // namespace connection_types

class ConnectionManager {
  public:
    ConnectionManager(const char* controlUnitPassword, RestClient& restClient);
    void    init();
    void    connect();
    bool    isConnectedToWiFi();
    bool    isPairedWithControlUnit();
    uint8_t getSensorId();

  private:
    void          checkFirmwareVersion();
    bool          connectToWiFi(const char* ssid);
    void          scanForUnits(const char* prefix = "CU-");
    void          tryToConnectControlUnit(const char* uuid = SENSOR_UNIT_ID);
    bool          m_isPaired;
    uint8_t       m_sensorId;
    unsigned long m_latestScan{0};
    unsigned long m_timeoutMs{10000};
    etl::vector<connection_types::cu_candidate, 8> m_candidateSsids;
    RestClient&                                    m_restClient;
    const char*                                    m_controlUnitPassword;
    static constexpr const char*                   TAG = "ConnectionManager";
};