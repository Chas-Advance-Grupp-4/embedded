/**
 * @file IScheduler.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Interface definition for a scheduler that triggers periodic system actions.
 * 
 * @date 2025-10-23
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once

/**
 * @brief Represents the result of a scheduler tick with trigger flags.
 */
struct SchedulerResult {
    bool connectTrigger;
    bool readingTrigger;
    bool dispatchTrigger;
    bool resyncTrigger;
};

/**
 * @brief Interface for a scheduler that triggers periodic actions.
 */
class IScheduler {
    public:
    /**
     * @brief Executes a scheduler tick and returns the actions to be triggered.
     * @param isPaired Indicates whether the device is currently paired.
     * @return A SchedulerResult containing trigger flags.
     */
    virtual SchedulerResult tick(bool isPaired) = 0;
    virtual ~IScheduler() = default;
};