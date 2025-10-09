#include "unity.h"
#include "JsonParser.h"

// Test UUID in valid format
constexpr const char* valid_uuid = "550e8400-e29b-41d4-a716-446655440000";

// Helper to create a sample reading
ca_sensorunit_reading createReading(uint32_t timestamp, float temperature, float humidity) {
    return {timestamp, temperature, humidity};
}

void setUp(void) {
}

void tearDown(void) {
}

void when_given_valid_readings_then_composeSensorSnapshotGroup_should_return_valid_json() {
    JsonParser parser;

    etl::vector<ca_sensorunit_reading, MAX_BATCH_SIZE> readings;
    readings.push_back(createReading(1726995600, 22.5f, 45.2f));
    readings.push_back(createReading(1726995660, 22.7f, 44.9f));

    etl::string<MAX_JSON_SIZE> result = parser.composeSensorSnapshotGroup(readings, valid_uuid);

    StaticJsonDocument<MAX_JSON_DOC_SIZE> doc;
    DeserializationError err = deserializeJson(doc, result.c_str());
    TEST_ASSERT_TRUE(err == DeserializationError::Ok);

    TEST_ASSERT_EQUAL_STRING(valid_uuid, doc["sensor_unit_id"]);
    JsonArray arr = doc["readings"];
    TEST_ASSERT_EQUAL(2, arr.size());

    TEST_ASSERT_EQUAL(1726995600, arr[0]["timestamp"]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 22.5f, arr[0]["temperature"]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 45.2f, arr[0]["humidity"]);

    TEST_ASSERT_EQUAL(1726995660, arr[1]["timestamp"]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 22.7f, arr[1]["temperature"]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 44.9f, arr[1]["humidity"]);
}

void when_given_empty_readings_then_composeSensorSnapshotGroup_should_return_json_with_empty_array() {
    JsonParser parser;

    etl::vector<ca_sensorunit_reading, MAX_BATCH_SIZE> readings;

    etl::string<MAX_JSON_SIZE> result = parser.composeSensorSnapshotGroup(readings, valid_uuid);

    StaticJsonDocument<MAX_JSON_DOC_SIZE> doc;
    DeserializationError err = deserializeJson(doc, result.c_str());
    TEST_ASSERT_TRUE(err == DeserializationError::Ok);

    TEST_ASSERT_EQUAL_STRING(valid_uuid, doc["sensor_unit_id"]);
    JsonArray arr = doc["readings"];
    TEST_ASSERT_EQUAL(0, arr.size());
}

void when_given_max_batch_size_then_composeSensorSnapshotGroup_should_return_json_with_all_entries() {
    JsonParser parser;

    etl::vector<ca_sensorunit_reading, MAX_BATCH_SIZE> readings;
    for (size_t i = 0; i < MAX_BATCH_SIZE; ++i) {
        readings.push_back(createReading(1726995600 + i * 60, 20.0f + i, 40.0f + i));
    }

    etl::string<MAX_JSON_SIZE> result = parser.composeSensorSnapshotGroup(readings, valid_uuid);

    StaticJsonDocument<MAX_JSON_DOC_SIZE> doc;
    DeserializationError err = deserializeJson(doc, result.c_str());
    TEST_ASSERT_TRUE(err == DeserializationError::Ok);

    JsonArray arr = doc["readings"];
    TEST_ASSERT_EQUAL(MAX_BATCH_SIZE, arr.size());

    for (size_t i = 0; i < MAX_BATCH_SIZE; ++i) {
        TEST_ASSERT_EQUAL(1726995600 + i * 60, arr[i]["timestamp"]);
        TEST_ASSERT_FLOAT_WITHIN(0.01, 20.0f + i, arr[i]["temperature"]);
        TEST_ASSERT_FLOAT_WITHIN(0.01, 40.0f + i, arr[i]["humidity"]);
    }
}


void when_given_null_uuid_then_composeSensorSnapshotGroup_should_return_json_with_null_id() {
    JsonParser parser;

    etl::vector<ca_sensorunit_reading, MAX_BATCH_SIZE> readings;
    readings.push_back(createReading(1726995600, 22.5f, 45.2f));

    // Pass null UUID
    etl::string<MAX_JSON_SIZE> result = parser.composeSensorSnapshotGroup(readings, nullptr);

    StaticJsonDocument<MAX_JSON_DOC_SIZE> doc;
    DeserializationError err = deserializeJson(doc, result.c_str());
    TEST_ASSERT_TRUE(err == DeserializationError::Ok);

    // Should be empty string if uuid is null
    TEST_ASSERT_TRUE(doc["sensor_unit_id"].isNull());
}

void when_given_extreme_sensor_values_then_composeSensorSnapshotGroup_should_handle_them_correctly() {
    JsonParser parser;

    etl::vector<ca_sensorunit_reading, MAX_BATCH_SIZE> readings;
    readings.push_back(createReading(0, -273.15f, 0.0f));         // Absolute zero
    readings.push_back(createReading(4294967295, 1000.0f, 100.0f)); // Max uint32 timestamp, extreme temp/humidity

    etl::string<MAX_JSON_SIZE> result = parser.composeSensorSnapshotGroup(readings, valid_uuid);

    StaticJsonDocument<MAX_JSON_DOC_SIZE> doc;
    DeserializationError err = deserializeJson(doc, result.c_str());
    TEST_ASSERT_TRUE(err == DeserializationError::Ok);

    JsonArray arr = doc["readings"];
    TEST_ASSERT_EQUAL(2, arr.size());

    TEST_ASSERT_EQUAL(0, arr[0]["timestamp"]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -273.15f, arr[0]["temperature"]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0f, arr[0]["humidity"]);

    uint32_t ts = arr[1]["timestamp"].as<uint32_t>();
    TEST_ASSERT_EQUAL_UINT32(4294967295u, ts);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1000.0f, arr[1]["temperature"]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 100.0f, arr[1]["humidity"]);
}

// Think about whether this is desired behaviour
void when_given_duplicate_timestamps_then_composeSensorSnapshotGroup_should_include_all_entries() {
    JsonParser parser;

    etl::vector<ca_sensorunit_reading, MAX_BATCH_SIZE> readings;
    readings.push_back(createReading(1726995600, 22.5f, 45.2f));
    readings.push_back(createReading(1726995600, 23.0f, 46.0f)); // Same timestamp

    etl::string<MAX_JSON_SIZE> result = parser.composeSensorSnapshotGroup(readings, valid_uuid);

    StaticJsonDocument<MAX_JSON_DOC_SIZE> doc;
    DeserializationError err = deserializeJson(doc, result.c_str());
    TEST_ASSERT_TRUE(err == DeserializationError::Ok);

    JsonArray arr = doc["readings"];
    TEST_ASSERT_EQUAL(2, arr.size());

    TEST_ASSERT_EQUAL(1726995600, arr[0]["timestamp"]);
    TEST_ASSERT_EQUAL(1726995600, arr[1]["timestamp"]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 22.5f, arr[0]["temperature"]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 23.0f, arr[1]["temperature"]);
}

void when_given_large_temperature_and_humidity_values_then_composeSensorSnapshotGroup_should_not_overflow() {
    JsonParser parser;

    etl::vector<ca_sensorunit_reading, MAX_BATCH_SIZE> readings;
    readings.push_back(createReading(1726995600, 999999.99f, 999999.99f));

    etl::string<MAX_JSON_SIZE> result = parser.composeSensorSnapshotGroup(readings, valid_uuid);

    StaticJsonDocument<MAX_JSON_DOC_SIZE> doc;
    DeserializationError err = deserializeJson(doc, result.c_str());
    TEST_ASSERT_TRUE(err == DeserializationError::Ok);

    JsonArray arr = doc["readings"];
    TEST_ASSERT_EQUAL(1, arr.size());

    TEST_ASSERT_FLOAT_WITHIN(0.01, 999999.99f, arr[0]["temperature"]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 999999.99f, arr[0]["humidity"]);
}



int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(when_given_valid_readings_then_composeSensorSnapshotGroup_should_return_valid_json);
  RUN_TEST(when_given_empty_readings_then_composeSensorSnapshotGroup_should_return_json_with_empty_array);
  RUN_TEST(when_given_max_batch_size_then_composeSensorSnapshotGroup_should_return_json_with_all_entries);
  RUN_TEST(when_given_null_uuid_then_composeSensorSnapshotGroup_should_return_json_with_null_id);
  RUN_TEST(when_given_extreme_sensor_values_then_composeSensorSnapshotGroup_should_handle_them_correctly);
  RUN_TEST(when_given_duplicate_timestamps_then_composeSensorSnapshotGroup_should_include_all_entries);
  RUN_TEST(when_given_large_temperature_and_humidity_values_then_composeSensorSnapshotGroup_should_not_overflow);
  return UNITY_END();
}


void setup() {
  // Wait ~2 seconds before the Unity test runner
  // establishes connection with a board Serial interface
  delay(2000);

  runUnityTests();
}

void loop() {}