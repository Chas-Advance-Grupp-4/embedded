#include "TimeSyncManager.h"
#include "JsonParser.h"
#include "logging.h"
#include "time.h"

TimeSyncManager::TimeSyncManager(RestClient& restClient) : m_restClient{restClient} {}

bool TimeSyncManager::isTimeSynced() {
    return m_isTimeSynced;
}

void TimeSyncManager::syncTime() {
    LOG_INFO(TAG, "Syncing time...");
    RestResponse response = m_restClient.getTo("/time");

    if (response.status == 200) {
        uint32_t currentTime = JsonParser::parseGetTimeResponse(response.payload);
        if (currentTime != 0) {
            m_baseTime   = currentTime;
            m_baseMillis = millis();
            m_isTimeSynced = true;
            LOG_INFO(TAG, "Time synced - Current Unix Time: %lu", currentTime);
            return;
        } else {
            LOG_WARN(TAG, "Error parsing JSON response from /time");
            LOG_WARN(TAG, "JSON payload: %s", response.payload.c_str());
        }
    } else {
        LOG_WARN(TAG, "Invalid response from /time - Status code %d", response.status);
    }
 
    if (m_isTimeSynced && (millis() - m_baseMillis) > m_maxSyncAgeMs) {
        m_isTimeSynced = false;
        LOG_WARN(TAG, "Time sync expired due to repeated failures");
    }
}

uint32_t TimeSyncManager::getUnixTimeNow() {
    if (isTimeSynced) {
        return m_baseTime + (millis() - m_baseMillis) / 1000;
    } else {
        LOG_WARN(TAG, "Time is not synced");
        return 0;
    }
}
