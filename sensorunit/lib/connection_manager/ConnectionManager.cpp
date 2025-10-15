#include "ConnectionManager.h"
#include "JsonParser.h"
#include "config.h"
#include "logging.h"
#include "WiFiS3.h"
#include "etl/algorithm.h"

ConnectionManager::ConnectionManager(RestClient& restClient)
    : m_restClient{restClient}, m_isPaired{false}, m_sensorId{0} {}

void ConnectionManager::init() {
    LOG_INFO(TAG, "Initializing...");

    // Initialize WiFi hardware
    if (WiFi.status() == WL_NO_MODULE) {
        LOG_ERROR(TAG, "WiFi module missing");
        return;
    }

    checkFirmwareVersion();

    WiFi.disconnect();

    // Restore internal status variables
    m_isPaired = false;
    m_sensorId = 0;

    LOG_INFO(TAG, "ConnectionManager initialized");
}

void ConnectionManager::connect() {
    LOG_INFO(TAG, "Running connect()");
    unsigned long       now          = millis();
    const unsigned long scanInterval = 30'000;

    if (m_candidateSsids.empty() || now - m_latestScan >= scanInterval) {
        scanForUnits("Zyxel");
    }

    bool connected = isConnectedToWiFi();
    if (!connected) {
        connected = connectToWiFi();
    }

    if (!m_isPaired) {
        tryToConnectControlUnit();
    }    
    LOG_INFO(TAG, "WiFi connected: %s, Paired: %s, Sensor ID: %d",
         connected ? "yes" : "no",
         m_isPaired ? "yes" : "no",
         m_sensorId);
}

bool ConnectionManager::isConnectedToWiFi() {
    return WiFi.status() == WL_CONNECTED;
}
bool ConnectionManager::isPairedWithControlUnit() {
    return m_isPaired;
}

void ConnectionManager::checkFirmwareVersion() {
    const char* fv = WiFi.firmwareVersion();

    auto parseVersion = [](const char* versionString) -> connection_types::fw_version {
        connection_types::fw_version v{0, 0, 0};
        sscanf(versionString, "%d.%d.%d", &v.major, &v.minor, &v.patch);
        return v;
    };

    connection_types::fw_version current = parseVersion(fv);
    connection_types::fw_version latest  = parseVersion(WIFI_FIRMWARE_LATEST_VERSION);

    if ((current.major < latest.major) ||
        (current.major == latest.major && current.minor < latest.minor) ||
        (current.major == latest.major && current.minor == latest.minor &&
         current.patch < latest.patch)) {
        LOG_WARN(
            TAG, "You have firmware %s, please update to %s", fv, WIFI_FIRMWARE_LATEST_VERSION);
    } else {
        LOG_INFO(TAG, "Firmware version: %s", fv);
    }
}

bool ConnectionManager::connectToWiFi() {
    LOG_INFO(TAG, "Connecting to WiFi...");
    WiFi.disconnect();
    WiFi.begin(WIFI_SECRET_SSID, WIFI_SECRET_PASS);

    unsigned long startAttemptTime = millis();
    int           status           = WiFi.status();
    while (status != WL_CONNECTED && millis() - startAttemptTime < m_timeoutMs) {
        delay(500);
        status = WiFi.status();
    }

    bool connected = (status == WL_CONNECTED);
    if (connected) {
        IPAddress localIp = WiFi.localIP();
        LOG_INFO(TAG,
                 "WiFi connected with IP: %d.%d.%d.%d",
                 localIp[0],
                 localIp[1],
                 localIp[2],
                 localIp[3]);
    } else {
        LOG_WARN(TAG, "WiFi connection failed with status: %d", WiFi.status());
    }
    return connected;
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
                    [](const connection_types::cu_candidate& a,
                       const connection_types::cu_candidate& b) { return a.rssi > b.rssi; });
}

void ConnectionManager::tryToConnectControlUnit(const char* uuid) {
    if (m_candidateSsids.empty()) {
        LOG_WARN(TAG, "No Candidate Control Units available for connection");
        return;
    }
    // Could probably be defined as constexpr in constants unless we need to add dynamic element
    etl::string<json_config::max_small_json_size> payload = JsonParser::composeConnectRequest(uuid);

    for (auto candidate : m_candidateSsids) {
        LOG_INFO(TAG, "Trying to connect to candidate: %s", candidate.ssid.c_str());
        RestResponse restResponse = m_restClient.postTo("/connect", payload);
        if (restResponse.status == 200) {
            ConnectResponse response = JsonParser::parseConnectResponse(restResponse.payload);
            if (response.connected) {
                m_isPaired = true;
                m_sensorId = response.sensorId;
                break;
            }
        }
    }
    if (!m_isPaired) {
        LOG_WARN(TAG, "Failed to connect to any Control Unit");
    }
    return;
}