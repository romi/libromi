#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "data_provider/JsonFieldNames.h"
#include "data_provider/RoverIdentityProvider.h"
#include "mock_softwareversion.h"
#include "mock_romidevicedata.h"

using namespace std;
using namespace testing;

class rover_identity_provider_tests : public ::testing::Test
{
protected:
        
	rover_identity_provider_tests() {
        }

	~rover_identity_provider_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {}
};

TEST_F(rover_identity_provider_tests, can_construct)
{
        // Arrange
        std::string expected_sw_current;
        std::string expected_sw_alternate;
        std::string expected_device_type;
        std::string expected_hardware_id;

        MockRomiDeviceData romiDeviceData;
        MockSoftwareVersion softwareVersion;

        EXPECT_CALL(romiDeviceData, RomiDeviceType())
                        .WillOnce(Return(expected_device_type));
        EXPECT_CALL(romiDeviceData, RomiDeviceHardwareId())
                        .WillOnce(Return(expected_hardware_id));
        EXPECT_CALL(softwareVersion, SoftwareVersionCurrent())
                        .WillOnce(Return(expected_sw_current));
        EXPECT_CALL(softwareVersion, SoftwareVersionAlternate())
                        .WillOnce(Return(expected_sw_alternate));

        // Act
        // Assert
        ASSERT_NO_THROW(RoverIdentityProvider identityProvider(romiDeviceData, softwareVersion));
}

TEST_F(rover_identity_provider_tests, will_create_identity)
{
        // Arrange

        std::string expected_sw_current = "V0.1RC1";
        std::string expected_sw_alternate = "N/A";
        std::string expected_device_type = "Rover_V1";
        std::string expected_hardware_id = "dead_beef_12fe_34ef";

        MockRomiDeviceData romiDeviceData;
        MockSoftwareVersion softwareVersion;

        EXPECT_CALL(romiDeviceData, RomiDeviceType())
                        .WillOnce(Return(expected_device_type));
        EXPECT_CALL(romiDeviceData, RomiDeviceHardwareId())
                        .WillOnce(Return(expected_hardware_id));
        EXPECT_CALL(softwareVersion, SoftwareVersionCurrent())
                        .WillOnce(Return(expected_sw_current));
        EXPECT_CALL(softwareVersion, SoftwareVersionAlternate())
                        .WillOnce(Return(expected_sw_alternate));

        RoverIdentityProvider identityProvider(romiDeviceData, softwareVersion);
        std::string identity = identityProvider.identity();

 //       "{\"identity\":{\"romi_hardware_id\":\"dead_beef_12fe_34ef\",\"romi_device_type\":\"Rover_V1\",\"software_version_alternate\":\"N/A\",\"software_version_current\":\"V0.1RC1\"}}"

        ASSERT_THAT(identity, HasSubstr(JsonFieldNames::identity.data()));
        ASSERT_THAT(identity, HasSubstr(JsonFieldNames::romi_device_type.data()));
        ASSERT_THAT(identity, HasSubstr(expected_device_type));
        ASSERT_THAT(identity, HasSubstr(JsonFieldNames::romi_hardware_id.data()));
        ASSERT_THAT(identity, HasSubstr(expected_hardware_id));
        ASSERT_THAT(identity, HasSubstr(JsonFieldNames::software_version_current.data()));
        ASSERT_THAT(identity, HasSubstr(expected_sw_current));
        ASSERT_THAT(identity, HasSubstr(JsonFieldNames::software_version_alternate.data()));
        ASSERT_THAT(identity, HasSubstr(expected_sw_alternate));

}