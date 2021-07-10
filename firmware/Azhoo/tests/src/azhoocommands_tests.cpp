#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../mocks/mock_azhoo.h"
#include "../mocks/mock_romiserial.h"
#include "AzhooCommands.h"

using namespace std;
using namespace testing;

static const int16_t NOTUSED = -1;

class azhoocommands_tests : public ::testing::Test
{
protected:
        MockAzhoo azhoo_;
        MockRomiSerial romi_serial_;
        std::string output_;
        
	azhoocommands_tests()
                : azhoo_(),
                  romi_serial_(),
                  output_() {
        }

	~azhoocommands_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {}

public:
	void set_output(const char *s) {
                output_ = s;
        }
};

TEST_F(azhoocommands_tests, test_setup)
{
        // Arrange
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        
        // Act
        setup_commands(&azhoo_, &romi_serial_);

        // Assert
}

TEST_F(azhoocommands_tests, handle_commands_calls_romi_serial)
{
        // Arrange
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(romi_serial_, handle_input());
        
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        handle_commands();

        // Assert
}

TEST_F(azhoocommands_tests, test_handle_commands_without_setup)
{
        // Arrange
        setup_commands(nullptr, nullptr);
        
        // Act
        handle_commands();

        // Assert
}

TEST_F(azhoocommands_tests, test_info)
{
        // Arrange
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(romi_serial_, send(_))
                .WillOnce(Invoke(this, &azhoocommands_tests::set_output));
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        send_info(&romi_serial_, nullptr, nullptr);

        // Assert
        ASSERT_TRUE(output_.find("[0,\"Azhoo\"") != std::string::npos);
}

TEST_F(azhoocommands_tests, test_encoders)
{
        // Arrange
        int32_t left = 1;
        int32_t right = 2;
        uint32_t time = 3;
        std::string expected("[0,1,2,3]");
        
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(azhoo_, get_encoders(_,_,_))
                .WillOnce(DoAll(SetArgReferee<0>(left),
                                SetArgReferee<1>(right),
                                SetArgReferee<2>(time)));
        EXPECT_CALL(romi_serial_, send(_))
                .WillOnce(Invoke(this, &azhoocommands_tests::set_output));
        
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        send_encoders(&romi_serial_, nullptr, nullptr);

        // Assert
        ASSERT_STREQ(expected.c_str(), output_.c_str());
}

TEST_F(azhoocommands_tests, test_successfull_configure_returns_ok)
{
        // Arrange
        int16_t params[] = { 10000, 100, NOTUSED, NOTUSED, 1, 1, NOTUSED, 1, -1 };
        
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(azhoo_, configure(_))
                .WillOnce(DoAll(Return(true)));
        EXPECT_CALL(romi_serial_, send_ok());
        
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        handle_configure(&romi_serial_, params, nullptr);

        // Assert
}

TEST_F(azhoocommands_tests, test_failed_configure_send_error)
{
        // Arrange
        int16_t params[] = { 10000, 100, NOTUSED, NOTUSED, 1, 1, NOTUSED, 1, -1 };
        
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(azhoo_, configure(_))
                .WillOnce(DoAll(Return(false)));
        EXPECT_CALL(romi_serial_, send_error(1, _));
        
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        handle_configure(&romi_serial_, params, nullptr);

        // Assert
}

TEST_F(azhoocommands_tests, test_successfull_enable_sends_ok)
{
        // Arrange
        int16_t params[] = { 1 };
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(azhoo_, enable())
                .WillOnce(DoAll(Return(true)));
        EXPECT_CALL(romi_serial_, send_ok());
        
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        handle_enable(&romi_serial_, params, nullptr);

        // Assert
}

TEST_F(azhoocommands_tests, test_successful_disable_sends_ok)
{
        // Arrange
        int16_t params[] = { 0 };
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(azhoo_, disable())
                .WillOnce(DoAll(Return(true)));
        EXPECT_CALL(romi_serial_, send_ok());
        
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        handle_enable(&romi_serial_, params, nullptr);

        // Assert
}

TEST_F(azhoocommands_tests, test_failed_enable_sends_error)
{
        // Arrange
        int16_t params[] = { 1 };
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(azhoo_, enable())
                .WillOnce(DoAll(Return(false)));
        EXPECT_CALL(romi_serial_, send_error(_, _));
        
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        handle_enable(&romi_serial_, params, nullptr);

        // Assert
}

TEST_F(azhoocommands_tests, test_failed_disable_sends_error)
{
        // Arrange
        int16_t params[] = { 0 };
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(azhoo_, disable())
                .WillOnce(DoAll(Return(false)));
        EXPECT_CALL(romi_serial_, send_error(_, _));
        
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        handle_enable(&romi_serial_, params, nullptr);

        // Assert
}

TEST_F(azhoocommands_tests, test_successful_moveat_sends_ok)
{
        // Arrange
        int16_t params[] = { 1, 1 };
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(azhoo_, set_target_speeds(10, 10))
                .WillOnce(DoAll(Return(true)));
        EXPECT_CALL(romi_serial_, send_ok());
        
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        handle_moveat(&romi_serial_, params, nullptr);

        // Assert
}

TEST_F(azhoocommands_tests, test_failed_moveat_sends_error)
{
        // Arrange
        int16_t params[] = { 1, 1 };
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(azhoo_, set_target_speeds(10, 10))
                .WillOnce(DoAll(Return(false)));
        EXPECT_CALL(romi_serial_, send_error(_,_));
        
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        handle_moveat(&romi_serial_, params, nullptr);

        // Assert
}

TEST_F(azhoocommands_tests, test_stop_sends_ok)
{
        // Arrange
        EXPECT_CALL(romi_serial_, set_handlers(_, _));
        EXPECT_CALL(azhoo_, stop());
        EXPECT_CALL(romi_serial_, send_ok());
        
        setup_commands(&azhoo_, &romi_serial_);
        
        // Act
        handle_stop(&romi_serial_, nullptr, nullptr);

        // Assert
}
