/**
 * @file TimeSyncManager.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Manages time synchronization with a remote server.
 * @date 2025-10-27
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once
#include "ITimeSyncManager.h"
#include "IRestClient.h"

/**
 * @brief Manages time synchronization with a remote server.
 *
 * This class implements ITimeSyncManager and provides functionality to synchronize
 * the system time using a REST API. It tracks whether the time is considered synced,
 * and calculates the current Unix time based on the last sync.
 */
class TimeSyncManager : public ITimeSyncManager {
  public:
    /**
     * @brief Constructs a TimeSyncManager with a reference to a RestClient.
     * @param restClient Reference to the RestClient used for time synchronization requests.
     */
    explicit TimeSyncManager(IRestClient& restClient);
    /**
     * @brief Checks if the system time is currently considered synchronized.
     * @return True if time is synced, false otherwise.
     */
    bool     isTimeSynced() override;
    /**
     * @brief Initiates a time synchronization request to the remote server.
     *
     * Updates internal time base if successful.
     */
    void     syncTime() override;
    /**
     * @brief Returns the current Unix time based on the last synchronization.
     * @return Estimated current Unix time in seconds.
     */
    uint32_t getUnixTimeNow() override;

  private:
    bool                         m_isTimeSynced{false};   /**< True if time has been successfully synced */
    uint32_t                     m_baseTime{0};           /**< Last synced Unix time in seconds */
    uint32_t                     m_baseMillis{0};         /**< Millis timestamp when time was last synced */
    uint32_t                     m_maxSyncAgeMs{ 60 * 60 * 1000 };   /**< Maximum age in ms before resync is needed */
    IRestClient&                  m_restClient;            /**< REST client used to fetch time from server */
    static constexpr const char* TAG = "TimeSyncManager"; /**< Logging tag for this class  */
};