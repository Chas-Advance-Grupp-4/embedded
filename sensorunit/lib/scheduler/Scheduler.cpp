/**
 * @file Scheduler.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implements the Scheduler logic for triggering actions like connect, read, dispatch, and
 * resync.
 *
 * * uint32_t now
 * Note:
 * If unit is not paired the variable now is based on milliseconds using millis()
 * Once unit is paired and time is synced the variable now uses Unix time seconds through the
 * timeSyncManager
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
                     uint32_t          syncTimeIntervalMs,
                     uint32_t          readingIntervalSec,
                     uint32_t          dispatchIntervalSec,
                     uint32_t          resyncTimeIntervalSec)
    : m_timeSyncManager(timeSyncManager), m_connectIntervalMs(connectIntervalMs),
      m_syncTimeIntervalMs(syncTimeIntervalMs), m_readingIntervalSec(readingIntervalSec),
      m_dispatchIntervalSec(dispatchIntervalSec), m_resyncTimeIntervalSec(resyncTimeIntervalSec) {}

SchedulerResult Scheduler::tick(bool isPaired) {
    SchedulerResult result{};
    uint32_t        now;
    if (!isPaired) {
        now = millis();
        if (now - m_lastConnectTimeMs > m_connectIntervalMs) {
            LOG_INFO(TAG, "Triggering connect attempt");
            m_lastConnectTimeMs   = now;
            result.connectTrigger = true;
        }
    } else if (!m_timeSyncManager.isTimeSynced()) {
        LOG_INFO(TAG, "Time not synced");
        now = millis();
        if (now - m_lastSyncTimeMs > m_syncTimeIntervalMs) {
            LOG_INFO(TAG, "Triggering time sync");
            m_lastSyncTimeMs = now;
            result.resyncTrigger = true;
        }
    } else {
        now = m_timeSyncManager.getUnixTimeNow();

        if (now % m_readingIntervalSec == 0 && now != m_lastReadingTimeUnix) {
            LOG_INFO(TAG, "Triggering sensor reading");
            m_lastReadingTimeUnix = now;
            result.readingTrigger = true;
        }

        if (now % m_dispatchIntervalSec == m_kdispatchOffset && now != m_lastDispatchTimeUnix) {
            LOG_INFO(TAG, "Triggering readings dispatch");
            m_lastDispatchTimeUnix = now;
            result.dispatchTrigger = true;
        }

        if (now % m_resyncTimeIntervalSec == m_kresyncOffset && now != m_lastResyncTimeUnix) {
            LOG_INFO(TAG, "Triggering time resync");
            m_lastResyncTimeUnix = now;
            result.resyncTrigger = true;
        }
    }
    return result;
}