/**
 * @file IReadingsDispatcher.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Interface for Readings Dispatcher
 * 
 * @date 2025-10-24
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once

class IReadingsDispatcher {
  public:
    virtual void dispatch() = 0;
    virtual ~IReadingsDispatcher() = default;
};