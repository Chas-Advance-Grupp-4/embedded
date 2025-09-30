extern "C" {
#include "unity.h"
}
#include "JsonParser.h"
#include <cstring>

extern "C" void when_readings_are_present_then_parseSensorSnapshotGroup_returns_all_snapshots(void) {
    std::string testJson = R"({
        "uuid": "550e8400-e29b-41d4-a716-446655440000",
        "readings": [
            {
                "timestamp": 1726995600,
                "temperature": 22.5,
                "humidity": 45.2
            },
            {
                "timestamp": 1726995660,
                "temperature": 22.7,
                "humidity": 44.9
            }
        ]
    })";

    auto snapshots = JsonParser::parseSensorSnapshotGroup(testJson);

    TEST_ASSERT_EQUAL_UINT32(2, snapshots.size());

    TEST_ASSERT_EQUAL_UINT32(1726995600, snapshots[0].timestamp);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 22.5, snapshots[0].temperature);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 45.2, snapshots[0].humidity);

    TEST_ASSERT_EQUAL_UINT32(1726995660, snapshots[1].timestamp);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 22.7, snapshots[1].temperature);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 44.9, snapshots[1].humidity);
}


extern "C" void when_grouped_readings_are_given_then_composeGroupedReadings_returns_expected_json(void) {

    // Construct an object to test
    std::map<time_t, std::vector<ca_sensorunit_snapshot>> readings;

    time_t ts1 = 1726995600;
    time_t ts2 = 1726995605;

    auto uuid1 = std::make_shared<Uuid>("550e8400-e29b-41d4-a716-446655440000");
    auto uuid2 = std::make_shared<Uuid>("123e4567-e89b-12d3-a456-426614174000");

    readings[ts1] = {
        {uuid1, ts1, 22.5, 45.2},
        {uuid2, ts1, 21.8, 47.0}
    };

    readings[ts2] = {
        {uuid1, ts2, 22.6, 45.1},
        {uuid2, ts2, 21.9, 46.8}
    };

    std::string json = JsonParser::composeGroupedReadings(readings);

    // check that the values match
    TEST_ASSERT_NOT_NULL(json.c_str());
    TEST_ASSERT_NOT_EQUAL(0, json.length());

    TEST_ASSERT_NOT_NULL(strstr(json.c_str(), "\"device_uuid\":\"f47ac10b-58cc-4372-a567-0e02b2c3d479\""));
    TEST_ASSERT_NOT_NULL(strstr(json.c_str(), "\"timestamp\":1726995600"));
    TEST_ASSERT_NOT_NULL(strstr(json.c_str(), "\"timestamp\":1726995605"));
    TEST_ASSERT_NOT_NULL(strstr(json.c_str(), "\"uuid\":\"550e8400-e29b-41d4-a716-446655440000\""));
    TEST_ASSERT_NOT_NULL(strstr(json.c_str(), "\"uuid\":\"123e4567-e89b-12d3-a456-426614174000\""));
    TEST_ASSERT_NOT_NULL(strstr(json.c_str(), "\"temperature\":22.5"));
    TEST_ASSERT_NOT_NULL(strstr(json.c_str(), "\"humidity\":45.2"));
    TEST_ASSERT_NOT_NULL(strstr(json.c_str(), "\"temperature\":21.9"));
    TEST_ASSERT_NOT_NULL(strstr(json.c_str(), "\"humidity\":46.8"));
}

extern "C" void when_sensor_connect_parseSensorConnect_returns_correct_request(void) {
    const std::string json = R"({
        "sensoruuid": "123e4567-e89b-12d3-a456-426614174000",
        "token": "secure-token-abc"
    })";

    SensorConnectRequest result = JsonParser::parseSensorConnectRequest(json, requestType::CONNECT);

    TEST_ASSERT_NOT_NULL(result.sensorUuid.get());
    TEST_ASSERT_EQUAL_STRING("123e4567-e89b-12d3-a456-426614174000", result.sensorUuid->toString().c_str());
    TEST_ASSERT_EQUAL_STRING("secure-token-abc", result.token.c_str());
    TEST_ASSERT_EQUAL(requestType::CONNECT, result.request);
}

extern "C" void when_sensor_connect_parseSensorConnect_returns_empty_on_missing_uuid(void) {
    const std::string json = R"({
        "token": "secure-token-abc"
    })";

    SensorConnectRequest result = JsonParser::parseSensorConnectRequest(json, requestType::CONNECT);

    TEST_ASSERT_NULL(result.sensorUuid.get());
}

extern "C" void when_sensor_connect_parseSensorConnect_returns_empty_on_invalid_json(void) {
    const std::string json = R"({ invalid json )";

    SensorConnectRequest result = JsonParser::parseSensorConnectRequest(json, requestType::CONNECT);

    TEST_ASSERT_NULL(result.sensorUuid.get());
}

extern "C" void when_sensor_connect_parseSensorConnect_returns_correct_request_for_disconnect(void) {
    const std::string json = R"({
        "sensoruuid": "987e6543-e21b-12d3-a456-426614174999",
        "token": "disconnect-token-xyz"
    })";

    SensorConnectRequest result = JsonParser::parseSensorConnectRequest(json, requestType::DISCONNECT);

    TEST_ASSERT_NOT_NULL(result.sensorUuid.get());
    TEST_ASSERT_EQUAL_STRING("987e6543-e21b-12d3-a456-426614174999", result.sensorUuid->toString().c_str());
    TEST_ASSERT_EQUAL_STRING("disconnect-token-xyz", result.token.c_str());
    TEST_ASSERT_EQUAL(requestType::DISCONNECT, result.request);
}