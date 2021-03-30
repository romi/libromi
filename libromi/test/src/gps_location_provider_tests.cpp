#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "data_provider/GpsLocationProvider.h"
#include "mock_gps.h"

using namespace std;
using namespace testing;

class gps_location_provider_tests : public ::testing::Test
{
protected:
        
	gps_location_provider_tests() {
        }

	~gps_location_provider_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {}
};

TEST_F(gps_location_provider_tests, can_construct)
{
        // Arrange
        MockGps gps;
        // Act
        // Assert
        ASSERT_NO_THROW(romi::GpsLocationProvider locationProvider(MockGps));
}

TEST_F(gps_location_provider_tests, will_create_location)
{
        // Arrange
        double latitude = 0.1;
        double longitude = 0.2;

        // Normally we use lat/long but the JSON parser retrieves things in reverse.
        std::string expected;
        expected =   R"({"location":{"longitude":)";
        expected += to_string(longitude);
        expected += R"(,"latitude":)";
        expected += to_string(latitude);
        expected += "}}";

        MockGps gps;
        EXPECT_CALL(gps, CurrentLocation(_,_))
                        .WillOnce(DoAll(SetArgReferee<0>(latitude), SetArgReferee<1>(longitude)));

        romi::GpsLocationProvider locationProvider(gps);
        std::string actual = locationProvider.location();
        ASSERT_EQ(actual, expected);
}