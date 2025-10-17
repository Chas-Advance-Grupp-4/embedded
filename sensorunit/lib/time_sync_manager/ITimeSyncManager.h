#pragma once
#include <stdint.h>

class ITimeSyncManager {
public:
	virtual bool isTimeSynced() = 0;
	virtual void syncTime() = 0;
	virtual uint32_t getUnixTimeNow() = 0;
};