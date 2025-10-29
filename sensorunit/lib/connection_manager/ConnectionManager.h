/**
 * @file ConnectionManager.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Class for handling connections with Control Unit
 *
 * The class scans for networks with a prefix
 * connects to WiFi and tries to connect with
 * a rest client POST to /connect
 *
 * @date 2025-10-26
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "RestClient.h"
#include "communication_data_types.h"
#include "config.h"
#include <Arduino.h>
#include <etl/string.h>
#include <etl/vector.h>

namespace connection_types {
/**
 * @brief Struct breaking down firmware version
 * for easier manipulation
 */
struct fw_version {
    int major;
    int minor;
    int patch;
};

/**
 * @brief Control Unit Candidates
 * holding SSID (name) and RSSI (signal strength)
 */
struct cu_candidate {
    etl::string<32> ssid;
    int             rssi;
};

}; // namespace connection_types

/**
 * @brief The class scans for networks with a prefix
 * connects to WiFi and tries to connect with
 * a rest client POST to /connect
 *
 */
class ConnectionManager {
  public:
    /**
     * @brief Construct a Connection Manager object
     *
     * @param controlUnitPassword password
     * @param restClient          reference to a restClient instance
     * @param sensorUnitId        The Uuid of this Sensor Unit
     */
    ConnectionManager(const char* controlUnitPassword,
                      RestClient& restClient,
                      const char* sensorUnitId = SENSOR_UNIT_ID);
    /**
     * @brief Initializes wifi
     * Also checks WiFi module, firmware and resets internal variables
     *
     */
    void init();
    /**
     * @brief Unified function for scanning for available control units
     * and connecting to them
     *
     */
    void connect();
    /**
     * @brief Disconnects Sensor Unit by running init
     * 
     */
    void disconnect();
    /**
     * @brief Helper function for checking if Sensor Unit is paired
     *
     * @return true when Sensor Unit is paired with a Control Unit
     */
    bool isPairedWithControlUnit();

  private:
    void          checkFirmwareVersion();
    bool          connectToWiFi(const char* ssid);
    void          scanForUnits(const char* prefix = "CU-");
    void          tryToConnectControlUnit();
    const char*                  m_controlUnitPassword; /**< Control Unit Password */
    RestClient&                  m_restClient;     /**< Reference to Rest Client */
    const char*   m_sensorUnitId;     /**< Sensor Unit UUID */
    bool          m_isPaired;         /**< true if Sensor Unit is paired*/
    unsigned long m_latestScan{0};    /**< Latest scan. Used by connect to rescan for units */
    unsigned long m_timeoutMs{10000}; /**< Timeout in Ms for WiFi connection */
    etl::vector<connection_types::cu_candidate, 8>
    m_candidateSsids; /**< Vector with SSID candidates. Max nr is 8 */
    static constexpr const char* TAG = "ConnectionManager";
};