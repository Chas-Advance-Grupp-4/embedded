/**
 * @file Scheduler.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @file Scheduler.h
 * @brief Concrete implementation of IScheduler that triggers actions based on time intervals.
 *
 * The reasoning behind this design is to keep it simple
 * avoiding the use of callbacks instead handling actions
 * with trigger flags in the main loop()
 *
 * A TimeSyncManager needs to be created first and passed in the constructor
 *
 * @date 2025-10-23
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "ConnectionManager.h"
#include "IScheduler.h"
#include "TimeSyncManager.h"

/**
 * @brief Scheduler implementation that triggers periodic actions based on time intervals.
 */
class Scheduler : public IScheduler {
  public:
    /**
     * @brief Constructs a Scheduler with specified time intervals.
     * @param timeSyncManager Reference to the time synchronization manager.
     * @param connectIntervalMs Interval for connection attempts in milliseconds.
     * @param syncTimeIntervalMs Interval for attempt to sync time in milliseconds.
     * @param readingIntervalSec Interval for sensor readings in seconds.
     * @param dispatchIntervalSec Interval for dispatching readings in seconds.
     * @param resyncTimeIntervalSec Interval for time resynchronization in seconds.
     */
    Scheduler(ITimeSyncManager& timeSyncManager,
              uint32_t          connectIntervalMs     = 5000,
              uint32_t          syncTimeIntervalMs    = 5000,
              uint32_t          readingIntervalSec    = 5,
              uint32_t          dispatchIntervalSec   = 15,
              uint32_t          resyncTimeIntervalSec = 10 * 60);
    /**
     * @brief Executes a scheduler tick and returns the actions to be triggered.
     * @param isPaired Indicates whether the device is currently paired.
     * @return A SchedulerResult containing trigger flags.
     */
    SchedulerResult tick(bool isPaired) override;

  private:
    uint32_t m_connectIntervalMs;        /**< Interval for connection attempts in milliseconds. */
    uint32_t m_syncTimeIntervalMs;       /**< Interval for syncing time in milliseconds. */ 
    uint32_t m_readingIntervalSec;       /**< Interval for sensor readings in seconds. */
    uint32_t m_dispatchIntervalSec;      /**< Interval for dispatching readings in seconds. */
    uint32_t m_resyncTimeIntervalSec;    /**< Interval for time resynchronization in seconds. */
    uint32_t m_lastConnectTimeMs;        /**< Millis Timestamp of the last connection attempt. */
    uint32_t m_lastSyncTimeMs;           /**< Millis Timestamp of the last sync time attempt. */
    uint32_t m_lastReadingTimeUnix;      /**< Unix Timestamp of the last sensor reading. */
    uint32_t m_lastDispatchTimeUnix;     /**< Unix Timestamp of the last data dispatch. */
    uint32_t m_lastResyncTimeUnix;       /**< Unix Timestamp of the last time resynchronization. */
    ITimeSyncManager& m_timeSyncManager; /**< Reference to the time synchronization manager. */
    static constexpr const uint32_t m_kdispatchOffset =
        1; /**< Offset in seconds for dispatch trigger */
    static constexpr const uint32_t m_kresyncOffset =
        2; /**< Offset in seconds for resync trigger */
    static constexpr const char* TAG = "Scheduler";
};
