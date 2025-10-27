/**
 * @file ITimeSyncManager.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Interface for time synchronization
 * @date 2025-10-27
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once
#include <stdint.h>

/**
 * @brief Interface for time synchronization
 *
 * Provides an abstraction for handling time syncing
 * Implementations are expected to track sync status and provide current Unix time.
 */
class ITimeSyncManager {
public:
    /**
     * @brief Checks whether the system time is currently considered synchronized.
     * @return True if time is synced, false otherwise.
     */
	virtual bool isTimeSynced() = 0;
	/**
     * @brief Initiates a time synchronization process.
     *
     * Implementations should update internal time tracking if the sync is successful.
     */
	virtual void syncTime() = 0;
	/**
     * @brief Should return the current Unix time in seconds.
     * @return current Unix time.
     */
	virtual uint32_t getUnixTimeNow() = 0;
};