/**
 * @brief Test file for SensorUnitManager.cpp
 *
 *
 */
extern "C" {
#include "unity.h"
}
#include "SensorUnitManager.h"

extern "C" void when_manager_empty_then_hasUnit_returns_false(void) {
    SensorUnitManager manager;
    manager.init();
    Uuid              uuid{"abc"};
    bool              hasUnit = manager.hasUnit(uuid);
    TEST_ASSERT_EQUAL(hasUnit, false);
}

extern "C" void when_unit_added_then_hasUnit_returns_true(void) {
    SensorUnitManager manager;
    manager.init();
    Uuid              uuid{"abc"};
    manager.addUnit(uuid);
    bool hasUnit = manager.hasUnit(uuid);
    TEST_ASSERT_EQUAL(hasUnit, true);
}

extern "C" void when_unit_added_and_removed_then_hasUnit_returns_false(void) {
    SensorUnitManager manager;
    manager.init();
    Uuid              uuid{"abc"};
    manager.addUnit(uuid);
    manager.removeUnit(uuid);
    bool hasUnit = manager.hasUnit(uuid);
    TEST_ASSERT_EQUAL(hasUnit, false);
}

extern "C" void when_unit_added_twice_then_logs_error(void) {
    SensorUnitManager manager;
    manager.init();
    Uuid              uuid{"abc"};
    manager.addUnit(uuid);
    manager.addUnit(uuid);
    TEST_ASSERT_EQUAL(manager.hasUnit(uuid), true);
}

extern "C" void when_nonexistent_unit_removed_then_logs_error(void) {
    SensorUnitManager manager;
    manager.init();
    Uuid              uuid{"xyz"};
    manager.removeUnit(uuid);
    TEST_ASSERT_EQUAL(manager.hasUnit(uuid), false);
}

extern "C" void stress_test_many_units(void) {
    SensorUnitManager manager;
    manager.init();
    for (int i = 0; i < 1000; ++i) {
        manager.addUnit(Uuid{std::to_string(i)});
    }
    TEST_ASSERT_EQUAL(manager.hasUnit(Uuid{"500"}), true);
}

static ca_sensorunit_snapshot
makeSnapshot(const std::string& uuid_str, time_t timestamp, double temp, double humidity) {
    auto uuid_ptr = std::make_shared<Uuid>(Uuid{uuid_str});
    return ca_sensorunit_snapshot{uuid_ptr, timestamp, temp, humidity};
}

static void assertSnapshotEqual(const ca_sensorunit_snapshot& expected,
                                const ca_sensorunit_snapshot& actual) {
    TEST_ASSERT_NOT_NULL(actual.uuid.get());
    TEST_ASSERT_EQUAL_STRING(expected.uuid->toString().c_str(), actual.uuid->toString().c_str());
    TEST_ASSERT_EQUAL(expected.timestamp, actual.timestamp);
    TEST_ASSERT_EQUAL(expected.temperature, actual.temperature);
    TEST_ASSERT_EQUAL(expected.humidity, actual.humidity);
}

extern "C" void when_reading_stored_then_it_is_grouped_by_timestamp(void) {
    SensorUnitManager manager;
    manager.init();
    manager.addUnit(Uuid{"abcde"});
    ca_sensorunit_snapshot snapshot = makeSnapshot("qwe", 1000, 25, 50);
    manager.storeReading(snapshot);
    auto grouped = manager.getGroupedReadings();

    TEST_ASSERT_NOT_EQUAL(grouped.end(), grouped.find(1000));
    TEST_ASSERT_EQUAL(1, grouped.size());
    TEST_ASSERT_EQUAL(1, grouped[1000].size());

    assertSnapshotEqual(snapshot, grouped[1000].at(0));
}

extern "C" void when_storing_multiple_readings_with_same_timestamp_then_grouped_together(void) {
    SensorUnitManager manager;
    manager.init();
    manager.addUnit(Uuid{"abcde"});

    manager.storeReading(makeSnapshot("qwe", 1000, 25, 50));
    manager.storeReading(makeSnapshot("qwe", 1000, 26, 51));
    auto grouped = manager.getGroupedReadings();

    TEST_ASSERT_EQUAL(1, grouped.size());
    TEST_ASSERT_TRUE(grouped.count(1000) > 0);
    TEST_ASSERT_EQUAL(2, grouped[1000].size());
}

extern "C" void when_storing_readings_with_different_timestamps_then_grouped_separately(void) {
    SensorUnitManager manager;
    manager.init();
    manager.addUnit(Uuid{"abcde"});

    manager.storeReading(makeSnapshot("qwe", 1000, 25, 50));
    manager.storeReading(makeSnapshot("qwe", 2000, 30, 60));
    auto grouped = manager.getGroupedReadings();

    TEST_ASSERT_EQUAL(2, grouped.size());
    TEST_ASSERT_TRUE(grouped.count(1000) > 0);
    TEST_ASSERT_TRUE(grouped.count(2000) > 0);
    TEST_ASSERT_EQUAL(1, grouped[1000].size());
    TEST_ASSERT_EQUAL(1, grouped[2000].size());
}

extern "C" void after_clearing_readings_grouped_readings_is_empty(void) {
    SensorUnitManager manager;
    manager.init();
    manager.addUnit(Uuid{"abcde"});
    manager.storeReading(makeSnapshot("qwe", 1000, 25, 50));
    manager.storeReading(makeSnapshot("qwe", 2000, 30, 60));

    auto grouped_before = manager.getGroupedReadings();
    TEST_ASSERT_EQUAL(2, grouped_before.size());

    manager.clearReadings();
    auto grouped_after = manager.getGroupedReadings();
    TEST_ASSERT_EQUAL(0, grouped_after.size());
}

extern "C" void after_clearing_one_reading_grouped_readings_contains_correct_amount(void) {
    SensorUnitManager manager;
    manager.init();
    manager.addUnit(Uuid{"abcde"});
    manager.storeReading(makeSnapshot("qwe", 1000, 25, 50));
    manager.storeReading(makeSnapshot("qwe", 2000, 30, 60));

    auto grouped_before = manager.getGroupedReadings();
    TEST_ASSERT_EQUAL_UINT(2, grouped_before.size());

    manager.clearReadings(1);
    auto grouped_after = manager.getGroupedReadings();
    TEST_ASSERT_EQUAL_UINT(1, grouped_after.size());
}