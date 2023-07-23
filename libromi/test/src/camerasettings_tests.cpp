#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "camera/CameraSettings.h"
#include <iostream>

using namespace std;
using namespace testing;
using namespace romi;

class camerasettings_tests : public ::testing::Test
{
protected:
        
	camerasettings_tests() {
        }

	~camerasettings_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {
        }
};

TEST_F(camerasettings_tests, test_constructor_1)
{
        // Arrange
        nlohmann::json json{
                {"name1", 1}, {"name2", 2}, {"name3", "value3"}
        };

        
        // Act
        try {
                CameraSettings settings("type", json);
        } catch (...) {
                FAIL() << "Didn't expected an error";
        }

        // Assert
}

TEST_F(camerasettings_tests, test_get_value)
{
        // Arrange
        nlohmann::json json = {
                {"name1", 1.0}, {"name2", 2.0}, {"name3", "value3"}
        };
        CameraSettings settings("type", json);

        //std::cout << json.dump(4) << std::endl;
        
        // Act
        double value1 = settings.get_value("name1");
        double value2 = settings.get_value("name2");
        
        // Assert
        ASSERT_EQ(value1, 1.0);
        ASSERT_EQ(value2, 2.0);
}

TEST_F(camerasettings_tests, test_get_value_fails_when_not_a_number)
{
        // Arrange
        nlohmann::json json = {
                {"name1", 1.0}, {"name2", 2.0}, {"name3", "value3"}
        };
        CameraSettings settings("type", json);
        
        // Act
        try {
                settings.get_value("name3");
                FAIL() << "Expected an error";
        } catch (...) {
                // Assert
        }
}

TEST_F(camerasettings_tests, test_get_value_fails_when_missing_number)
{
        // Arrange
        nlohmann::json json = {
                {"name1", 1.0}, {"name2", 2.0}, {"name3", "value3"}
        };
        CameraSettings settings("type", json);
        
        // Act
        try {
                settings.get_value("missing");
                FAIL() << "Expected an error";
        } catch (...) {
                // Assert
        }
}

TEST_F(camerasettings_tests, test_get_option)
{
        // Arrange
        nlohmann::json json = {
                {"name1", 1.0}, {"name2", 2.0}, {"name3", "value3"}
        };
        CameraSettings settings("type", json);

        //std::cout << json.dump(4) << std::endl;
        
        // Act
        std::string value;
        settings.get_option("name3", value);
        
        // Assert
        ASSERT_STREQ(value.c_str(), "value3");
}

TEST_F(camerasettings_tests, test_get_option_fails_when_not_a_string)
{
        // Arrange
        nlohmann::json json = {
                {"name1", 1.0}, {"name2", 2.0}, {"name3", "value3"}
        };
        CameraSettings settings("type", json);

        //std::cout << json.dump(4) << std::endl;
        
        // Act
        try {
                std::string value;
                settings.get_option("name1", value);
                FAIL() << "Expected an error";
        } catch (...) {
                // Assert
        }
}

TEST_F(camerasettings_tests, test_get_option_fails_when_missing)
{
        // Arrange
        nlohmann::json json = {
                {"name1", 1.0}, {"name2", 2.0}, {"name3", "value3"}
        };
        CameraSettings settings("type", json);

        //std::cout << json.dump(4) << std::endl;
        
        // Act
        try {
                std::string value;
                settings.get_option("missing", value);
                FAIL() << "Expected an error";
        } catch (...) {
                // Assert
        }
}

TEST_F(camerasettings_tests, test_set_get_value)
{
        // Arrange
        nlohmann::json json = {
                {"name1", 1.0}, {"name2", 2.0}, {"name3", "value3"}
        };
        CameraSettings settings("type", json);
        settings.set_value("name1", 10.0);

        
        // Act
        double value1 = settings.get_value("name1");
        
        // Assert
        ASSERT_EQ(value1, 10.0);
}

TEST_F(camerasettings_tests, test_select_get_option)
{
        // Arrange
        nlohmann::json json = {
                {"name1", 1.0}, {"name2", 2.0}, {"name3", "value3"}
        };
        CameraSettings settings("type", json);
        settings.select_option("name3", "new value");
        
        // Act
        std::string value;
        settings.get_option("name3", value);
        
        // Assert
        ASSERT_STREQ(value.c_str(), "new value");
}
