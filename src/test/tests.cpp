#include "catch2/catch_test_macros.hpp"
#include "RangeStore.hpp"

// Minimal requirements for the key.
struct TestKey
{
    int value;
    TestKey() = delete;

    TestKey(int val) : value(val) {}
    TestKey(TestKey const &Other)
    {
        value = Other.value;
    }

    TestKey &operator=(TestKey const &other) = default;
    bool operator<(TestKey const &other) const
    {
        return value < other.value;
    }
};

// Minimal requirements for values
struct TestValue
{
    int data;
    TestValue() = delete;

    TestValue(int val) : data(val) {}
    TestValue(TestValue const &Other)
    {
        data = Other.data;
    }

    TestValue &operator=(TestValue const &other) = default;
    bool operator==(TestValue const &other) const
    {
        return data == other.data;
    }
};

TEST_CASE("RangeStore can be initialized")
{
    range_store<int, char>{'a'};
}

TEST_CASE("All fields are set to the initial value")
{
    range_store<int, char> store{'a'};

    SECTION("Positive values")
    {
        REQUIRE(store[5] == 'a');
        REQUIRE(store[100] == 'a');
        REQUIRE(store[9999] == 'a');
    }

    SECTION("Negative values")
    {
        REQUIRE(store[-1] == 'a');
        REQUIRE(store[-100] == 'a');
        REQUIRE(store[-9999] == 'a');
    }

    SECTION("zero")
    {
        REQUIRE(store[0] == 'a');
    }
}

TEST_CASE("WORKS WITH MINIMAL KEYS & VALUES")
{
    range_store<TestKey, TestValue> store(TestValue{0});

    store.assign(TestKey{-5}, TestKey{5}, TestValue{10});

    REQUIRE(store[TestKey{0}] == TestValue{10});
}

TEST_CASE("SHOULD DEAL WITH DUPLICATE KEYS")
{
    range_store<int, char> store{'a'};

    // Values [0, 10) should have 'b', the rest 'a'
    store.assign(0,2, 'b');
    store.assign(1,3, 'b');
    store.assign(1,7, 'b');
    store.assign(3,10, 'b');
    store.assign(3,7, 'b');

    REQUIRE(store[-1] == 'a');

    for (int i = 0; i < 10; i++)
    {
        REQUIRE(store[i] == 'b');
    }
    
    REQUIRE(store[10] == 'a');
}

TEST_CASE("SHOULD WORK WITH VERY LARGE VALUES")
{
    range_store<int, char> store{'a'};

    store.assign(std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 'b');

    assert(store[0] == 'b');
    assert(store[std::numeric_limits<int>::min()] == 'b');
    // The end of the range isn't included, so we can't actually set the max value.
    assert(store[std::numeric_limits<int>::max()] == 'a');
}

TEST_CASE("SHOULD OVERWRITE VALUES IN NEW RANGES")
{
    range_store<int, char> store{'a'};

    store.assign(0, 10, 'b');
    store.assign(-5, 5, 'c');
    store.assign(5, 15, 'd');

    // We expect all the 'b' values to be gone.

    for (int i = -5; i < 5; i++)
    {
        REQUIRE(store[i] == 'c');
    }
    for (int i = 5; i < 15; i++)
    {
        REQUIRE(store[i] == 'd');
    }
}

TEST_CASE("SHOULD ALLOW EMBEDDING RANGES IN OTHER RANGES")
{
    range_store<int, char> store{'a'};
    store.assign(0, 15, 'b');
    store.assign(5, 10, 'c');

    // The middle values should be 'c', the rest 'b'
    for (int i = 0; i < 5; i++)
    {
        REQUIRE(store[i] == 'b');
    }
    for (int i = 5; i < 10; i++)
    {
        REQUIRE(store[i] == 'c');
    }
    for (int i = 10; i < 15; i++)
    {
        REQUIRE(store[i] == 'b');
    }
}