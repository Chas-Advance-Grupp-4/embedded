/**
 * @brief main file for the test_runner
 *
 * This is for running unit tests. It is still just an example
 * For now it runs them on the hardware
 * Will be separated into native logic tests and also Dockerized later on
 *
 */
extern "C" {
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <unity.h>
}

#define LOG_TEST_GROUP(name)                                                   \
    ESP_LOGI("TEST", "========== Testing %s ==========", name)

extern "C" {

void when_manager_empty_then_hasUnit_returns_false(void);
void when_unit_added_then_hasUnit_returns_true(void);
void when_unit_added_and_removed_then_hasUnit_returns_false(void);
void when_unit_added_twice_then_logs_error(void);
void when_nonexistent_unit_removed_then_logs_error(void);
void stress_test_many_units(void);
void when_reading_stored_then_it_is_grouped_by_timestamp(void);
void when_storing_multiple_readings_with_same_timestamp_then_grouped_together(
    void);
void when_storing_readings_with_different_timestamps_then_grouped_separately(
    void);
void after_clearing_readings_grouped_readings_is_empty(void);
// JsonParser
void when_passed_a_uuid_composeStatusRequest_generates_valid_json(void);
void when_passed_empty_string_composeStatusRequest_returns_empty_string(void);
void when_given_valid_json_parseStatusResponse_returns_correct_response(void);
void when_given_valid_delivered_parseStatusResponse_returns_disconnect_response(void);
void when_given_invalid_json_parseStatusResponse_returns_empty_vector(void);
void when_given_valid_json_parseBackendReadingsResponse_returns_correct_value(void);
void when_given_invalid_json_parseBackendReadingsResponse_returns_zero(void);
void when_status_not_ok_parseBackendReadingsResponse_returns_zero(void);

void when_readings_are_present_then_parseSensorSnapshotGroup_returns_all_snapshots(
    void);
void when_grouped_readings_are_given_then_composeGroupedReadings_returns_expected_json(
    void);
void when_valid_connect_json_is_given_then_parseSensorConnectRequest_returns_expected_request(
    void);
void when_sensor_uuid_is_missing_then_parseSensorConnectRequest_returns_empty_request(
    void);
void when_json_is_invalid_then_parseSensorConnectRequest_returns_empty_request(
    void);
void when_valid_disconnect_json_is_given_then_parseSensorConnectRequest_returns_expected_request(
    void);
void when_valid_sensor_response_is_given_then_composeSensorConnectResponse_returns_expected_json(
    void);
void when_sensor_uuid_is_missing_then_composeSensorConnectResponse_sets_uuid_to_unknown(
    void);
void when_connection_status_is_pending_then_composeSensorConnectResponse_serializes_status_correctly(
    void);
// connectionStatusToString
void when_given_connected_status_connectionStatusToString_returns_connected(
    void);
void when_given_disconnected_status_connectionStatusToString_returns_disconnected(
    void);
void when_given_pending_status_connectionStatusToString_returns_pending(void);
void when_given_unavailable_status_connectionStatusToString_returns_unavailable(
    void);
void when_given_invalid_status_connectionStatusToString_returns_unknown(void);

// composeErrorResponse
void when_passing_message_to_composeErrorResponse_then_it_should_return_valid_json_string(
    void);
} // extern "C"

// Lägg till testen i main
extern "C" void app_main() {
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    esp_log_level_set("*", ESP_LOG_INFO);
    ESP_LOGI("TEST", "Test-runner startar...");

    LOG_TEST_GROUP("SensorUnitManager");
    UNITY_BEGIN();
    RUN_TEST(when_manager_empty_then_hasUnit_returns_false);
    RUN_TEST(when_unit_added_then_hasUnit_returns_true);
    RUN_TEST(when_unit_added_and_removed_then_hasUnit_returns_false);
    RUN_TEST(when_unit_added_twice_then_logs_error);
    RUN_TEST(when_nonexistent_unit_removed_then_logs_error);
    // RUN_TEST(stress_test_many_units);
    RUN_TEST(when_reading_stored_then_it_is_grouped_by_timestamp);
    RUN_TEST(
        when_storing_multiple_readings_with_same_timestamp_then_grouped_together);
    RUN_TEST(
        when_storing_readings_with_different_timestamps_then_grouped_separately);
    RUN_TEST(after_clearing_readings_grouped_readings_is_empty);

    LOG_TEST_GROUP("JsonParser");
    RUN_TEST(when_passed_a_uuid_composeStatusRequest_generates_valid_json);
    RUN_TEST(when_passed_empty_string_composeStatusRequest_returns_empty_string);
    RUN_TEST(when_given_valid_json_parseStatusResponse_returns_correct_response);
    RUN_TEST(when_given_valid_delivered_parseStatusResponse_returns_disconnect_response);
    RUN_TEST(when_given_invalid_json_parseStatusResponse_returns_empty_vector);
    RUN_TEST(when_given_valid_json_parseBackendReadingsResponse_returns_correct_value);
    RUN_TEST(when_given_invalid_json_parseBackendReadingsResponse_returns_zero);
    RUN_TEST(when_status_not_ok_parseBackendReadingsResponse_returns_zero);
    RUN_TEST(
        when_readings_are_present_then_parseSensorSnapshotGroup_returns_all_snapshots);
    RUN_TEST(
        when_grouped_readings_are_given_then_composeGroupedReadings_returns_expected_json);
    RUN_TEST(
        when_valid_connect_json_is_given_then_parseSensorConnectRequest_returns_expected_request);
    RUN_TEST(
        when_sensor_uuid_is_missing_then_parseSensorConnectRequest_returns_empty_request);
    RUN_TEST(
        when_json_is_invalid_then_parseSensorConnectRequest_returns_empty_request);
    RUN_TEST(
        when_valid_disconnect_json_is_given_then_parseSensorConnectRequest_returns_expected_request);
    RUN_TEST(
        when_valid_sensor_response_is_given_then_composeSensorConnectResponse_returns_expected_json);
    RUN_TEST(
        when_sensor_uuid_is_missing_then_composeSensorConnectResponse_sets_uuid_to_unknown);
    RUN_TEST(
        when_connection_status_is_pending_then_composeSensorConnectResponse_serializes_status_correctly);
    // connectionStatusToString
    RUN_TEST(
        when_given_connected_status_connectionStatusToString_returns_connected);
    RUN_TEST(
        when_given_disconnected_status_connectionStatusToString_returns_disconnected);
    RUN_TEST(
        when_given_pending_status_connectionStatusToString_returns_pending);
    RUN_TEST(
        when_given_unavailable_status_connectionStatusToString_returns_unavailable);
    RUN_TEST(
        when_given_invalid_status_connectionStatusToString_returns_unknown);

    // composeErrorResponse
    RUN_TEST(
        when_passing_message_to_composeErrorResponse_then_it_should_return_valid_json_string);

    UNITY_END();
}
