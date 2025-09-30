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

// Deklarera testfunktionen från komponenten
extern "C" void when_manager_empty_then_hasUnit_returns_false(void);
extern "C" void when_unit_added_then_hasUnit_returns_true(void);
extern "C" void when_unit_added_and_removed_then_hasUnit_returns_false(void);
extern "C" void when_unit_added_twice_then_logs_error(void);
extern "C" void when_nonexistent_unit_removed_then_logs_error(void);
extern "C" void stress_test_many_units(void);
extern "C" void when_reading_stored_then_it_is_grouped_by_timestamp(void);
extern "C" void when_storing_multiple_readings_with_same_timestamp_then_grouped_together(void);
extern "C" void when_storing_readings_with_different_timestamps_then_grouped_separately(void);
extern "C" void after_clearing_readings_grouped_readings_is_empty(void);
// JsonParser
extern "C" void when_readings_are_present_then_parseSensorSnapshotGroup_returns_all_snapshots(void);
extern "C" void when_grouped_readings_are_given_then_composeGroupedReadings_returns_expected_json(void);
extern "C" void when_sensor_connect_parseSensorConnect_returns_correct_request(void);
extern "C" void when_sensor_connect_parseSensorConnect_returns_empty_on_missing_uuid(void);
extern "C" void when_sensor_connect_parseSensorConnect_returns_empty_on_invalid_json(void);
extern "C" void when_sensor_connect_parseSensorConnect_returns_correct_request_for_disconnect(void);

// Lägg till testen i main
extern "C" void app_main() {
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    esp_log_level_set("*", ESP_LOG_INFO);
    ESP_LOGI("TEST", "Test-runner startar...");

    ESP_LOGI("TEST", "Testing SensorUnitManager");
    UNITY_BEGIN();
    RUN_TEST(when_manager_empty_then_hasUnit_returns_false);
    RUN_TEST(when_unit_added_then_hasUnit_returns_true);
    RUN_TEST(when_unit_added_and_removed_then_hasUnit_returns_false);
    RUN_TEST(when_unit_added_twice_then_logs_error);
    RUN_TEST(when_nonexistent_unit_removed_then_logs_error);
    // RUN_TEST(stress_test_many_units);
    RUN_TEST(when_reading_stored_then_it_is_grouped_by_timestamp);
    RUN_TEST(when_storing_multiple_readings_with_same_timestamp_then_grouped_together);
    RUN_TEST(when_storing_readings_with_different_timestamps_then_grouped_separately);
    RUN_TEST(after_clearing_readings_grouped_readings_is_empty);
    UNITY_END();

    ESP_LOGI("TEST", "Testing JsonParser");
    UNITY_BEGIN();
    RUN_TEST(when_readings_are_present_then_parseSensorSnapshotGroup_returns_all_snapshots);
    RUN_TEST(when_grouped_readings_are_given_then_composeGroupedReadings_returns_expected_json);
    RUN_TEST(when_sensor_connect_parseSensorConnect_returns_correct_request);
    RUN_TEST(when_sensor_connect_parseSensorConnect_returns_empty_on_missing_uuid);
    RUN_TEST(when_sensor_connect_parseSensorConnect_returns_empty_on_missing_uuid);
    RUN_TEST(when_sensor_connect_parseSensorConnect_returns_empty_on_invalid_json);
    RUN_TEST(when_sensor_connect_parseSensorConnect_returns_correct_request_for_disconnect);
    UNITY_END();
}
