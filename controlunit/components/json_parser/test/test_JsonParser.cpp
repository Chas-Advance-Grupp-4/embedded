extern "C" {
#include "unity.h"
}
#include "JsonParser.h"

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
