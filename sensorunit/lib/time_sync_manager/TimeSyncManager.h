#pragma once
#include "ITimeSyncManager.h"
#include "RestClient.h"

class TimeSyncManager : public ITimeSyncManager {
  public:
    TimeSyncManager(RestClient& restClient);
    bool     isTimeSynced() override;
    void     syncTime() override;
    uint32_t getUnixTimeNow() override;

  private:
    bool                         m_isTimeSynced{false};
    uint32_t                     m_baseTime{0};
    uint32_t                     m_baseMillis{0};
    uint32_t                     m_maxSyncAgeMs{ 60 * 60 * 1000 };
    RestClient&                  m_restClient;
    static constexpr const char* TAG = "TimeSyncManager";
};