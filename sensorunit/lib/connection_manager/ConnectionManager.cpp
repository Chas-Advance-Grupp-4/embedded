#include "ConnectionManager.h"
#include "logging.h"
#include "WiFiS3.h"

ConnectionManager::ConnectionManager()
     {}

void ConnectionManager::init() {
    LOG_INFO(TAG, "Initializing...");
// Initialize WiFi hardware
    if (WiFi.status() == WL_NO_MODULE) {
        LOG_ERROR(TAG, "WiFi module missing");
        return;
    }

    // Check firmware version
    const char* fv = WiFi.firmwareVersion();

    auto parseVersion = [](const char* versionStr) -> fwVersion {
        fwVersion v{0, 0, 0};
        sscanf(versionStr, "%d.%d.%d", &v.major, &v.minor, &v.patch);
        return v;
    };

    fwVersion current = parseVersion(fv);
    fwVersion latest = parseVersion(WIFI_FIRMWARE_LATEST_VERSION);

    if ((current.major < latest.major) ||
        (current.major == latest.major && current.minor < latest.minor) ||
        (current.major == latest.major && current.minor == latest.minor && current.patch < latest.patch)) {
        LOG_WARN(TAG, "You have firmware %s, please update to %s", fv, WIFI_FIRMWARE_LATEST_VERSION);
    } else {
        LOG_INFO(TAG, "Firmware version: %s", fv);
    }

    // Initialize REST client 
    // m_restClient.begin(); 

    // Restore internal status variables
    m_isPaired = false;
    m_ip = IPAddress();  

    LOG_INFO(TAG, "ConnectionManager initialized");
}


	ControlUnitInfo ConnectionManager::connect() {
        return {};
    }
	bool ConnectionManager::isConnectedToWiFi() {
        return false;
    }
	bool ConnectionManager::isPairedWithControlUnit() {
        return false;
    }
	IPAddress ConnectionManager::getIp(){
        return {};
    }

    ControlUnitInfo ConnectionManager::scanForUnits() {
        return {};
    }
	ControlUnitInfo ConnectionManager::sendConnectRequest(IPAddress ip) {
        return {};
    }