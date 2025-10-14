#include "ConnectionManager.h"
#include "WiFiS3.h"
#include "etl/algorithm.h"
#include "logging.h"

ConnectionManager::ConnectionManager() {}

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
    fwVersion latest  = parseVersion(WIFI_FIRMWARE_LATEST_VERSION);

    if ((current.major < latest.major) ||
        (current.major == latest.major && current.minor < latest.minor) ||
        (current.major == latest.major && current.minor == latest.minor &&
         current.patch < latest.patch)) {
        LOG_WARN(
            TAG, "You have firmware %s, please update to %s", fv, WIFI_FIRMWARE_LATEST_VERSION);
    } else {
        LOG_INFO(TAG, "Firmware version: %s", fv);
    }

    // Initialize REST client
    // m_restClient.begin();

    // Restore internal status variables
    m_isPaired = false;
    m_ip       = IPAddress();

    LOG_INFO(TAG, "ConnectionManager initialized");
}

ControlUnitInfo ConnectionManager::connect() {
    unsigned long now = millis();
    const unsigned long scanInterval = 30'000;

    if (m_candidateSsids.empty() || now - m_latestScan >= scanInterval) {
        scanForUnits("Zyxel");
    }
    return {};
}
bool ConnectionManager::isConnectedToWiFi() {
    return false;
}
bool ConnectionManager::isPairedWithControlUnit() {
    return false;
}
IPAddress ConnectionManager::getIp() {
    return {};
}

void ConnectionManager::scanForUnits(const char* prefix) {
    LOG_INFO(TAG, "Scanning for Units...");
    m_latestScan = millis();
    m_candidateSsids.clear();

    int numNetworks = WiFi.scanNetworks();
    LOG_INFO(TAG, "%d networks found", numNetworks);
    if (numNetworks <= 0) {
        LOG_WARN(TAG, "No networks found");
        return;
    }
    for (int i = 0; i < numNetworks; ++i) {
        etl::string<32> ssid = WiFi.SSID(i);
        int             rssi = WiFi.RSSI(i);
        if (ssid.starts_with(prefix)) {
            LOG_INFO(TAG, "Found candidate Control Unit: %s (RSSI: %d)", ssid.c_str(), rssi);
            if (m_candidateSsids.full()) {
                LOG_WARN(TAG, "Too many SSID");
            } else {
                m_candidateSsids.push_back({ssid, rssi});
            }
        }
    }
    etl::shell_sort(m_candidateSsids.begin(),
                    m_candidateSsids.end(),
                    [](const Candidate& a, const Candidate& b) { return a.rssi > b.rssi; });
}

ControlUnitInfo ConnectionManager::sendConnectRequest(IPAddress ip) {
    return {};
}