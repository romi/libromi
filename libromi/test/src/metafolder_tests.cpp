#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ClockAccessor.h"
#include "data_provider/RoverIdentityProvider.h"
#include "data_provider/GpsLocationProvider.h"
#include "weeder_session/MetaFolder.h"
#include "Linux.h"
#include "FileUtils.h"


#include "mock_romidevicedata.h"
#include "mock_softwareversion.h"
#include "mock_linux.h"
#include "mock_clock.h"
#include "mock_gps.h"

using namespace std;
using namespace testing;

class metafolder_tests : public ::testing::Test
{
protected:
        
	metafolder_tests() : mockClock_(std::make_shared<rpp::MockClock>()), mockGps_(),
	mockLocationProvider_(), deviceData_(), softwareVersion_(), sessions_basedir_("metafolder_test"),
	homedir_("."), versionCurrent_("V1.0.1"), versionAlternate_("V1.0.1"), devicetype_("Rover"), devicID_("DEAD-BEEF"),
	sesion_path_("./session")
	{
        }

	~metafolder_tests() override = default;

	void SetUp() override {
                rpp::ClockAccessor::SetInstance(mockClock_);
                mockLocationProvider_ = std::make_unique<romi::GpsLocationProvider>(mockGps_);
                if (fs::is_directory(sesion_path_))
                {
                        fs::remove_all(sesion_path_);
                }
        }

	void TearDown() override {
                rpp::ClockAccessor::SetInstance(nullptr);
	}

	void SetDeviceIDDataExpectations(const std::string& deviceType, const std::string& deviceId, int times)
        {
                EXPECT_CALL(deviceData_, RomiDeviceType)
                                .Times(times)
                                .WillRepeatedly(Return(deviceType));
                EXPECT_CALL(deviceData_, RomiDeviceHardwareId)
                                .Times(times)
                                .WillRepeatedly(Return(deviceId));
        }

        void SetSoftwareVersionDDataExpectations(const std::string& versionCurrent, const std::string& versionAlternate)
        {
                EXPECT_CALL(softwareVersion_, SoftwareVersionCurrent)
                            .WillOnce(Return(versionCurrent));
                EXPECT_CALL(softwareVersion_, SoftwareVersionAlternate)
                    .WillOnce(Return(versionAlternate));
        }

        std::filesystem::path BuildSessionDirName(const std::string& deviceType, const std::string& deviceId, std::string& date_time)
        {
                std::string separator("_");
                std::filesystem::path session_dir = homedir_;
                session_dir /= sessions_basedir_;
                session_dir /= deviceType + separator + deviceId + separator + date_time;
	        return session_dir;
        }

        std::shared_ptr<rpp::MockClock> mockClock_;
        MockGps mockGps_;
        std::unique_ptr<romi::ILocationProvider> mockLocationProvider_;
        MockRomiDeviceData deviceData_;
        MockSoftwareVersion softwareVersion_;
        const std::string sessions_basedir_;
        const std::string homedir_;
        const std::string versionCurrent_;
        const std::string versionAlternate_;
        const std::string devicetype_;
        const std::string devicID_;
        const std::filesystem::path sesion_path_;
};

TEST_F(metafolder_tests, construct_throws_when_roveridentity_null)
{
      // Arrange
      auto mockLocationProvider_ = std::make_unique<romi::GpsLocationProvider>(mockGps_);
      std::unique_ptr<romi::RoverIdentityProvider> roverIdentity;

      // Act
      // Assert
      ASSERT_THROW(romi::MetaFolder meta_folder(std::move(roverIdentity), std::move(mockLocationProvider_)), std::invalid_argument);
}

TEST_F(metafolder_tests, construct_throws_when_locationprovider_null)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        std::unique_ptr<romi::GpsLocationProvider> mockLocationProvider_;
        auto roverIdentity = std::make_unique<romi::RoverIdentityProvider>(deviceData_, softwareVersion_);

        // Act
        // Assert
        ASSERT_THROW(romi::MetaFolder meta_folder(std::move(roverIdentity), std::move(mockLocationProvider_)), std::invalid_argument);
}

TEST_F(metafolder_tests, can_construct)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_unique<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_unique<romi::RoverIdentityProvider>(deviceData_, softwareVersion_);

        // Act
        // Assert
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(std::move(roverIdentity), std::move(mockLocationProvider_)));
}

TEST_F(metafolder_tests, try_create_creates_folder)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_unique<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_unique<romi::RoverIdentityProvider>(deviceData_, softwareVersion_);
        romi::MetaFolder meta_folder(std::move(roverIdentity), std::move(mockLocationProvider_));

        // Act
        // Assert
        ASSERT_NO_THROW(meta_folder.try_create(sesion_path_));
        ASSERT_TRUE(fs::is_directory(sesion_path_));
}

TEST_F(metafolder_tests, try_create_creates_metadata_file)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_unique<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_unique<romi::RoverIdentityProvider>(deviceData_, softwareVersion_);
        romi::MetaFolder meta_folder(std::move(roverIdentity), std::move(mockLocationProvider_));

        // Act
        // Assert
        ASSERT_NO_THROW(meta_folder.try_create(sesion_path_));
        ASSERT_TRUE(fs::is_directory(sesion_path_));
}