#pragma once
#include "communication_data_types.h"
#include <Arduino.h>
#include <etl/string.h>
#include <etl/vector.h>

struct fwVersion {
    int major;
    int minor;
    int patch;
};


class ConnectionManager {
  public:
    ConnectionManager();
  	void init();
	ControlUnitInfo connect();
	bool isConnectedToWiFi();
	bool isPairedWithControlUnit();
	IPAddress getIp();
  private: 
    struct Candidate {
        etl::string<32> ssid;
        int rssi;
    };

	void scanForUnits(const char* prefix = "CU-");
    bool tryToConnectControlUnit(const char* uuid);
	ControlUnitInfo sendConnectRequest(IPAddress ip);
	bool m_isPaired;
    unsigned long m_latestScan { 0 };
	IPAddress m_ip;
    etl::vector<Candidate, 8> m_candidateSsids;
	// RestClient& m_restClient;
    static constexpr const char* TAG = "ConnectionManager";
};