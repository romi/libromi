#include <ClockAccessor.h>
#include <data_provider/GpsLocationProvider.h>
#include "Linux.h"
#include "FileUtils.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "weeder/Session.h"
#include "mock_romidevicedata.h"
#include "mock_softwareversion.h"
#include "mock_linux.h"
#include "mock_clock.h"
#include "mock_gps.h"

using namespace std;
using namespace testing;

uint8_t red_image[] = { 0,0,0,  32,0,0,  64,0,0,  96,0,0,
                             32,0,0, 64,0,0,  96,0,0,  128,0,0,
                             64,0,0, 96,0,0,  128,0,0, 160,0,0,
                             96,0,0, 128,0,0, 160,0,0, 192,0,0 };

class weedersession : public ::testing::Test
{
protected:
        
	weedersession() : mockClock_(std::make_shared<rpp::MockClock>()), mockGps_(),
	mockLocationProvider_(), deviceData_(), softwareVersion_(), sessions_basedir_("weedersession_test"),
	homedir_("."), versionCurrent_("V1.0.1"), versionAlternate_("V1.0.1"), devicetype_("Rover"), devicID_("DEAD-BEEF")
	{
        }

	~weedersession() override = default;

	void SetUp() override {
                rpp::ClockAccessor::SetInstance(mockClock_);
                mockLocationProvider_ = std::make_unique<romi::GpsLocationProvider>(mockGps_);
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
};

TEST_F(weedersession, can_construct_with_valid_directory)
{
        // Arrange
        rpp::Linux linux;
        std::string session_dir = "weedersession_test";

        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto weeder_session_dir = FileUtils::TryGetHomeDirectory(linux);
        weeder_session_dir /= session_dir;
        std::filesystem::remove_all(weeder_session_dir);

        // Act
        // Assert
        ASSERT_NO_THROW(romi::Session session(linux, session_dir, deviceData_, softwareVersion_, std::move(mockLocationProvider_)));
}

TEST_F(weedersession, can_construct_when_directory_exists)
{
        // Arrange
        rpp::MockLinux mock_linux;

        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        EXPECT_CALL(mock_linux, secure_getenv)
                        .Times(2)
                        .WillRepeatedly(Return((char*)homedir_.c_str()));

        std::string session_dir = "weedersession_test";
        auto weeder_session_dir = FileUtils::TryGetHomeDirectory(mock_linux);
        weeder_session_dir /= session_dir;
        std::filesystem::remove_all(weeder_session_dir);
        auto create = std::filesystem::create_directories(weeder_session_dir);

        // Act
        // Assert
        ASSERT_NO_THROW(romi::Session session(mock_linux, session_dir, deviceData_, softwareVersion_, std::move(mockLocationProvider_)));
        ASSERT_TRUE(create);
}

TEST_F(weedersession, fail_construct_when_fails_to_create_directory)
{
        // Arrange
        rpp::MockLinux mock_linux;

        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        std::string expected("/");
        EXPECT_CALL(mock_linux, secure_getenv)
                        .WillOnce(Return((char*)expected.c_str()));

        std::string session_dir = "weedersession_test";
        bool exception_thrown = false;

        // Act
        try {
                romi::Session session(mock_linux, session_dir, deviceData_, softwareVersion_, std::move(mockLocationProvider_));
        }
        catch(std::runtime_error& e){
                std::cout << e.what();
                exception_thrown = true;
        }

        // Assert
        ASSERT_TRUE(exception_thrown);
}

TEST_F(weedersession, start_creates_correct_directory)
{
        // Arrange
        rpp::MockLinux mock_linux;
        EXPECT_CALL(mock_linux, secure_getenv)
                        .WillOnce(Return((char*)homedir_.c_str()));
        SetDeviceIDDataExpectations(devicetype_, devicID_, 2);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        std::string date_time("01012025");
        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .WillOnce(Return(date_time));

        std::string observation_id("obs_id_1");

        std::filesystem::path session_dir = BuildSessionDirName(devicetype_, devicID_, date_time);

        if (fs::is_directory(session_dir))
        {
                fs::remove_all(session_dir);
        }

        // Act
        try {
                romi::Session session(mock_linux, sessions_basedir_, deviceData_, softwareVersion_, std::move(mockLocationProvider_));
                session.start(observation_id);
        }
        catch(std::runtime_error& e){
                std::cout << e.what();
        }

        // Assert
        ASSERT_TRUE(fs::is_directory(session_dir));
}

TEST_F(weedersession, current_directory_correct)
{
        // Arrange
        rpp::MockLinux mock_linux;
        EXPECT_CALL(mock_linux, secure_getenv)
                        .WillOnce(Return((char*)homedir_.c_str()));
        SetDeviceIDDataExpectations(devicetype_, devicID_, 2);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        std::string date_time("01012025");
        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .WillOnce(Return(date_time));

        std::string observation_id("obs_id_1");

        std::filesystem::path session_dir = BuildSessionDirName(devicetype_, devicID_, date_time);
        fs::remove_all(session_dir);
        std::filesystem::path current_path;
        // Act
        try {
                romi::Session session(mock_linux, sessions_basedir_, deviceData_, softwareVersion_, std::move(mockLocationProvider_));
                session.start(observation_id);
                current_path = session.current_path();

        }
        catch(std::runtime_error& e){
                std::cout << e.what();
        }

        // Assert
        ASSERT_TRUE(fs::is_directory(session_dir));
        ASSERT_EQ(current_path, session_dir);
}

TEST_F(weedersession, store_functions_store_files)
{
        // Arrange
        rpp::MockLinux mock_linux;
        EXPECT_CALL(mock_linux, secure_getenv)
                        .WillOnce(Return((char*)homedir_.c_str()));
        SetDeviceIDDataExpectations(devicetype_, devicID_, 2);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        std::string date_time("01012025");
        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .WillRepeatedly(Return(date_time));

        EXPECT_CALL(mockGps_, CurrentLocation)
                        .WillRepeatedly(DoAll(testing::SetArgReferee<0>(0.1),testing::SetArgReferee<1>(0.2)));

        std::string observation_id("obs_id_1");
        std::filesystem::path session_dir = BuildSessionDirName(devicetype_, devicID_, date_time);
        fs::remove_all(session_dir);
        romi::Image image(romi::Image::RGB, red_image, 4, 4);
        std::string svg_body("svg_body");

        double data(0.0);
        romi::Path testPath;
        for (int i = 0; i < 10; i++) {
                data+=0.1;
                testPath.emplace_back(romi::v3(data, data, 0.0));
        }
        // Act
        romi::Session session(mock_linux, sessions_basedir_, deviceData_, softwareVersion_, std::move(mockLocationProvider_));
        session.start(observation_id);
        session.store_jpg("file", image);
        session.store_png("file", image);
        session.store_svg("file", svg_body);
        session.store_txt("file", svg_body);
        session.store_path("file", 0, testPath);
        session.store_path("file", 1, testPath);

        // Assert
        ASSERT_TRUE(fs::is_directory(session_dir));
        ASSERT_TRUE(fs::exists(session_dir/"file.jpg"));
        ASSERT_TRUE(fs::exists(session_dir/"file.png"));
        ASSERT_TRUE(fs::exists(session_dir/"file.svg"));
        ASSERT_TRUE(fs::exists(session_dir/"file.txt"));
        ASSERT_TRUE(fs::exists(session_dir/"file.path"));
        ASSERT_TRUE(fs::exists(session_dir/"file-00001.path"));
}


TEST_F(weedersession, store_functions_throw_on_failure)
{
        // Arrange
        rpp::MockLinux mock_linux;
        EXPECT_CALL(mock_linux, secure_getenv)
                        .WillOnce(Return((char*)homedir_.c_str()));
        SetDeviceIDDataExpectations(devicetype_, devicID_, 2);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        std::string date_time("01012025");
        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .WillRepeatedly(Return(date_time));

        EXPECT_CALL(mockGps_, CurrentLocation)
                        .WillRepeatedly(DoAll(testing::SetArgReferee<0>(0.1),testing::SetArgReferee<1>(0.2)));

        std::string observation_id("obs_id_1");
        std::filesystem::path session_dir = BuildSessionDirName(devicetype_, devicID_, date_time);
        fs::remove_all(session_dir);
        romi::Image image(romi::Image::RGB, red_image, 4, 4);
        std::string svg_body("svg_body");

        double data(0.0);
        romi::Path testPath;
        for (int i = 0; i < 10; i++) {
                data+=0.1;
                testPath.emplace_back(romi::v3(data, data, 0.0));
        }

        // Act
        // Assert
        romi::Session session(mock_linux, sessions_basedir_, deviceData_, softwareVersion_, std::move(mockLocationProvider_));
        session.start(observation_id);
        ASSERT_ANY_THROW(session.store_jpg("file&%//-*", image));
        ASSERT_ANY_THROW(session.store_png("file&%//-*", image));
        ASSERT_ANY_THROW(session.store_svg("file&%//-*", svg_body));
        ASSERT_ANY_THROW(session.store_txt("file&%//-*", svg_body));
        ASSERT_ANY_THROW(session.store_path("file&%//-*", 0, testPath));
        ASSERT_ANY_THROW(session.store_path("file&%//-*", 1, testPath));

}


