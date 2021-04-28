#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "BrushMotorDriver.h"
#include "mock_romiserialclient.h"

using namespace std;
using namespace testing;
using namespace romi;

const int default_encoder_steps = 123;
const double default_maximum_revolutions_per_second = 1.7;

class brushmotordriver_tests : public ::testing::Test
{
protected:
        vector<string> expected_output;
        vector<string> observed_output;
        vector<string> mock_response;
        JsonCpp driver_config;
        int encoder_steps;
        double maximum_revolutions_per_second;
        
	brushmotordriver_tests()
	: expected_output(),
          observed_output(),
          mock_response(),
          driver_config(),
          encoder_steps(default_encoder_steps),
          maximum_revolutions_per_second(default_maximum_revolutions_per_second) {
                const char * config_string = "{"
                        "'maximum_signal_amplitude': 71,"
                        "'use_pid': false,"
                        "'pid': {'kp': 1.1, 'ki': 2.2, 'kd': 3.3},"
                        "'encoder_directions': {'left': -1, 'right': 1 }}";
                driver_config = JsonCpp::parse(config_string);
	}

	~brushmotordriver_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}

        void append_output(const char *s, JsonCpp& response) {
                size_t index = observed_output.size();
                observed_output.emplace_back(s);
                response = JsonCpp::parse(mock_response[index].c_str());
        }

        void add_expected_output(MockRomiSerialClient& serial,
                                 const char *command,
                                 const char *response) {
                expected_output.emplace_back(command);
                mock_response.emplace_back(response);
                EXPECT_CALL(serial, send(_,_))
                        .WillOnce(Invoke(this, &brushmotordriver_tests::append_output))
                        .RetiresOnSaturation();
        }
};

TEST_F(brushmotordriver_tests, parse_config)
{
        // Arrange
        BrushMotorDriverSettings settings{};

        // Act
        settings.parse(driver_config);
        
        //Assert
        ASSERT_EQ(settings.max_signal, 71);
        ASSERT_EQ(settings.use_pid, false);
        ASSERT_EQ(settings.kp, 1.1);
        ASSERT_EQ(settings.ki, 2.2);
        ASSERT_EQ(settings.kd, 3.3);
        ASSERT_EQ(settings.dir_left, -1);
        ASSERT_EQ(settings.dir_right, 1);
}

TEST_F(brushmotordriver_tests, successful_config_and_enable)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[0]");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);

        // Act
        BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                maximum_revolutions_per_second);

        // Assert
        ASSERT_EQ(expected_output.size(), observed_output.size());
        for (size_t i = 0; i < expected_output.size(); i++) {
                ASSERT_STREQ(observed_output[i].c_str(),
                             expected_output[i].c_str());
        }
}

TEST_F(brushmotordriver_tests, throws_exception_at_failed_config)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[1]");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);

        try {
                // Act
                BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                        maximum_revolutions_per_second);
                FAIL() << "Expected std::runtime_error";
        }  catch (std::runtime_error const &e) {
                // Assert
                EXPECT_STREQ(e.what(), "BrushMotorDriver: Initialization failed");
        } catch (...) {
                FAIL() << "Expected std::runtime_error";
        }
}

TEST_F(brushmotordriver_tests, throws_exception_at_failed_enable)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[1]");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);

        try {
                // Act
                BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                        maximum_revolutions_per_second);
                FAIL() << "Expected std::runtime_error";
        }  catch (std::runtime_error const &e) {
                
                // Assert
                EXPECT_STREQ(e.what(), "BrushMotorDriver: Initialization failed");
                ASSERT_EQ(expected_output.size(), observed_output.size());
                ASSERT_STREQ(observed_output[0].c_str(),
                             expected_output[0].c_str());

        } catch (...) {
                FAIL() << "Expected std::runtime_error";
        }
}

TEST_F(brushmotordriver_tests, returns_true_on_successful_moveat)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[0]");
        add_expected_output(*mock_serial, "V[100,200]", "[0]");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);
        
        BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                maximum_revolutions_per_second);

        // Act
        bool success = driver.moveat(0.1, 0.2);

        // Assert
        ASSERT_EQ(expected_output.size(), observed_output.size());
        for (size_t i = 0; i < expected_output.size(); i++) {
                ASSERT_STREQ(observed_output[i].c_str(),
                             expected_output[i].c_str());
        }
        ASSERT_EQ(success, true);
}

TEST_F(brushmotordriver_tests, returns_false_on_unsuccessful_moveat)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[0]");
        add_expected_output(*mock_serial, "V[100,200]", "[1,'Just fooling you']");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);
        
        BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                maximum_revolutions_per_second);
        
        // Act
        bool success = driver.moveat(0.1, 0.2);
        
        // Assert
        ASSERT_EQ(expected_output.size(), observed_output.size());
        for (size_t i = 0; i < expected_output.size(); i++) {
                ASSERT_STREQ(observed_output[i].c_str(),
                             expected_output[i].c_str());
        }
        
        ASSERT_EQ(success, false);
}

TEST_F(brushmotordriver_tests, returns_true_on_successful_get_encoders)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[0]");
        add_expected_output(*mock_serial, "e", "[0,100,200,300]");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);

        BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                maximum_revolutions_per_second);
        
        // Act
        double left, right, timestamp;
        bool success = driver.get_encoder_values(left, right, timestamp);
        
        // Assert
        ASSERT_EQ(expected_output.size(), observed_output.size());
        for (size_t i = 0; i < expected_output.size(); i++) {
                ASSERT_STREQ(observed_output[i].c_str(),
                             expected_output[i].c_str());
        }
        ASSERT_EQ(success, true);
        ASSERT_EQ(left, 100.0);
        ASSERT_EQ(right, 200.0);
        ASSERT_EQ(timestamp, 0.300);
}

TEST_F(brushmotordriver_tests, returns_false_on_unsuccessful_get_encoders)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[0]");
        add_expected_output(*mock_serial, "e", "[1,'TEST']");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);

        BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                maximum_revolutions_per_second);
        
        // Act
        double left, right, timestamp;
        bool success = driver.get_encoder_values(left, right, timestamp);
        
        // Assert
        ASSERT_EQ(expected_output.size(), observed_output.size());
        for (size_t i = 0; i < expected_output.size(); i++) {
                ASSERT_STREQ(observed_output[i].c_str(),
                             expected_output[i].c_str());
        }
        ASSERT_EQ(success, false);
}

TEST_F(brushmotordriver_tests, returns_false_on_invalid_speeds_1)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[0]");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);

        BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                maximum_revolutions_per_second);
        
        // Act
        bool success = driver.moveat(2.0, 0.0);
        
        // Assert
        ASSERT_EQ(expected_output.size(), observed_output.size());
        for (size_t i = 0; i < expected_output.size(); i++) {
                ASSERT_STREQ(observed_output[i].c_str(),
                             expected_output[i].c_str());
        }
        ASSERT_EQ(success, false);
}

TEST_F(brushmotordriver_tests, returns_false_on_invalid_speeds_2)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[0]");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);

        BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                maximum_revolutions_per_second);
        
        // Act
        bool success = driver.moveat(-2.0, 0.0);
        
        // Assert
        ASSERT_EQ(expected_output.size(), observed_output.size());
        for (size_t i = 0; i < expected_output.size(); i++) {
                ASSERT_STREQ(observed_output[i].c_str(),
                             expected_output[i].c_str());
        }
        ASSERT_EQ(success, false);
}

TEST_F(brushmotordriver_tests, returns_false_on_invalid_speeds_3)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[0]");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);

        BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                maximum_revolutions_per_second);
        
        // Act
        bool success = driver.moveat(0.0, -1.1);
        
        // Assert
        ASSERT_EQ(expected_output.size(), observed_output.size());
        for (size_t i = 0; i < expected_output.size(); i++) {
                ASSERT_STREQ(observed_output[i].c_str(),
                             expected_output[i].c_str());
        }
        ASSERT_EQ(success, false);
}

TEST_F(brushmotordriver_tests, returns_false_on_invalid_speeds_4)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[0]");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);

        BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                maximum_revolutions_per_second);
        
        // Act
        bool success = driver.moveat(0.0, 1.1);
        
        // Assert
        ASSERT_EQ(expected_output.size(), observed_output.size());
        for (size_t i = 0; i < expected_output.size(); i++) {
                ASSERT_STREQ(observed_output[i].c_str(),
                             expected_output[i].c_str());
        }
        ASSERT_EQ(success, false);
}

TEST_F(brushmotordriver_tests, returns_true_on_successful_stop)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[0]");
        add_expected_output(*mock_serial, "X", "[0]");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);

        BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                maximum_revolutions_per_second);
        
        // Act
        bool success = driver.stop();
        
        // Assert
        ASSERT_EQ(expected_output.size(), observed_output.size());
        for (size_t i = 0; i < expected_output.size(); i++) {
                ASSERT_STREQ(observed_output[i].c_str(),
                             expected_output[i].c_str());
        }
        ASSERT_EQ(success, true);
}

TEST_F(brushmotordriver_tests, returns_false_on_failed_stop)
{
        // Arrange
        auto mock_serial = std::make_unique<MockRomiSerialClient>();
        add_expected_output(*mock_serial, "C[123,170,71,0,1100,2200,3300,-1,1]", "[0]");
        add_expected_output(*mock_serial, "E[1]", "[0]");
        add_expected_output(*mock_serial, "X", "[1,\"error\"]");
        std::unique_ptr<romiserial::IRomiSerialClient> serial = std::move(mock_serial);

        BrushMotorDriver driver(serial, driver_config, encoder_steps,
                                maximum_revolutions_per_second);
        
        // Act
        bool success = driver.stop();
        
        // Assert
        ASSERT_EQ(expected_output.size(), observed_output.size());
        for (size_t i = 0; i < expected_output.size(); i++) {
                ASSERT_STREQ(observed_output[i].c_str(),
                             expected_output[i].c_str());
        }
        ASSERT_EQ(success, false);
}
