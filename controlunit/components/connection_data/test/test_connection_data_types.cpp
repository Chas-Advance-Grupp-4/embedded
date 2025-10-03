extern "C" {
#include "unity.h"
}
#include "connection_data_types.h"


extern "C" {

void when_given_connected_status_connectionStatusToString_returns_connected(void) {
    TEST_ASSERT_EQUAL_STRING("connected", connectionStatusToString(connectionStatus::CONNECTED).c_str());
}

void when_given_disconnected_status_connectionStatusToString_returns_disconnected(void) {
    TEST_ASSERT_EQUAL_STRING("disconnected", connectionStatusToString(connectionStatus::DISCONNECTED).c_str());
}

void when_given_pending_status_connectionStatusToString_returns_pending(void) {
    TEST_ASSERT_EQUAL_STRING("pending", connectionStatusToString(connectionStatus::PENDING).c_str());
}

void when_given_unavailable_status_connectionStatusToString_returns_unavailable(void) {
    TEST_ASSERT_EQUAL_STRING("unavailable", connectionStatusToString(connectionStatus::UNAVAILABLE).c_str());
}

void when_given_invalid_status_connectionStatusToString_returns_unknown(void) {
    TEST_ASSERT_EQUAL_STRING("unknown", connectionStatusToString(static_cast<connectionStatus>(999)).c_str());
}

}