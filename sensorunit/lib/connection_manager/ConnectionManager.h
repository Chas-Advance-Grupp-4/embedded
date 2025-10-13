#pragma once
#include "communication_data_types.h"
#include <Arduino.h>

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
	ControlUnitInfo scanForUnits();
	ControlUnitInfo sendConnectRequest(IPAddress ip);
	bool m_isPaired;
	IPAddress m_ip;
	// RestClient& m_restClient;
    static constexpr const char* TAG = "ConnectionManager";
};