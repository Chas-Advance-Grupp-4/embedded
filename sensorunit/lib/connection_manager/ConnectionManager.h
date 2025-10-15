#pragma once
#include "RestClient.h"
#include "communication_data_types.h"
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
    ConnectionManager(RestClient& restClient);
    void            init();
    ConnectResponse connect();
    bool            isConnectedToWiFi();
    bool            isPairedWithControlUnit();

  private:
    void                                           checkFirmwareVersion();
    bool                                           connectToWiFi();
    void                                           scanForUnits(const char* prefix = "CU-");
    ConnectResponse                                tryToConnectControlUnit(const char* uuid);
    bool                                           m_isPaired;
    unsigned long                                  m_latestScan{0};
    unsigned long                                  m_timeoutMs{10000};
    etl::vector<connection_types::cu_candidate, 8> m_candidateSsids;
    RestClient&                                    m_restClient;
    static constexpr const char*                   TAG = "ConnectionManager";
};