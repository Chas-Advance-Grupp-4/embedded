/**
 * @file Scheduler.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implements the Scheduler logic for triggering actions like connect, read, dispatch, and resync.
 * 
 * * uint32_t now
 * Note:
 * If time is not synced the variable now is based on milliseconds using millis()
 * Once time IS synced the variable now uses Unix time seconds through the timeSyncManager
 * 
 * @date 2025-10-23
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#include "Scheduler.h"
#include "Arduino.h"
#include "logging.h"

Scheduler::Scheduler(ITimeSyncManager& timeSyncManager,
                     uint32_t          connectIntervalMs,
                     uint32_t          readingIntervalSec,
                     uint32_t          dispatchIntervalSec,
                     uint32_t          resyncTimeIntervalSec)
    : m_timeSyncManager(timeSyncManager), m_connectIntervalMs(connectIntervalMs),
      m_readingIntervalSec(readingIntervalSec), m_dispatchIntervalSec(dispatchIntervalSec),
      m_resyncTimeIntervalSec(resyncTimeIntervalSec) {}

SchedulerResult Scheduler::tick(bool isPaired) {
    SchedulerResult result{};
    uint32_t        now;
    if (!m_timeSyncManager.isTimeSynced()) {
        now = millis();
        if (!isPaired && now - m_connectIntervalMs > m_lastConnectTimeMs) {
            LOG_INFO(TAG, "Triggering connect attempt");
            m_lastConnectTimeMs   = now;
            result.connectTrigger = true;
        }
    } else {
        now = m_timeSyncManager.getUnixTimeNow();

        if (now % m_readingIntervalSec == 0 && now != m_lastReadingTimeUnix) {
            LOG_INFO(TAG, "Triggering sensor reading");
            m_lastReadingTimeUnix = now;
            result.readingTrigger = true;
        }

        if (now % m_dispatchIntervalSec == 1 && now != m_lastDispatchTimeUnix) {
            LOG_INFO(TAG, "Triggering readings dispatch");
            m_lastDispatchTimeUnix = now;
            result.dispatchTrigger = true;
        }

        if (now % m_resyncTimeIntervalSec == 2 && now != m_lastResyncTimeUnix) {
            LOG_INFO(TAG, "Triggering time resync");
            m_lastResyncTimeUnix = now;
            result.resyncTrigger = true;
        }
    }
    return result;
}