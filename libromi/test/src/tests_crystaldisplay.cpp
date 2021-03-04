#include <linux/joystick.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../mock/mock_romiserialclient.h"
#include "CrystalDisplay.h"

using namespace std;
using namespace testing;
using namespace romi;

class crystaldisplay_tests : public ::testing::Test
{
protected:
        MockRomiSerialClient romi_serial;
        std::string firmware_reply;
        std::string observed_output;
        
	crystaldisplay_tests() : romi_serial(), firmware_reply(), observed_output(){
	}

	~crystaldisplay_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}

        void set_reply(const char *s) {
                firmware_reply = s;
        }
        
public:
        void set_output(const char *s, JsonCpp& response) {
                observed_output = s;
                response = JsonCpp::parse(firmware_reply.c_str());
        }
};

TEST_F(crystaldisplay_tests, assure_count_lines_is_two)
{
        CrystalDisplay display(romi_serial);
        ASSERT_EQ(display.count_lines(), 2);
}

TEST_F(crystaldisplay_tests, show_invalid_line_returns_false)
{
        CrystalDisplay display(romi_serial);
        ASSERT_EQ(display.show(display.count_lines(), "foo"), false);
}

TEST_F(crystaldisplay_tests, clear_invalid_line_returns_fals)
{
        CrystalDisplay display(romi_serial);
        ASSERT_EQ(display.clear(display.count_lines()), false);
}

TEST_F(crystaldisplay_tests, show_produces_a_valid_command)
{
        set_reply("[0]");
        EXPECT_CALL(romi_serial, send(_,_))
                .WillOnce(Invoke(this, &crystaldisplay_tests::set_output))
                .RetiresOnSaturation();

        CrystalDisplay display(romi_serial);
        bool success = display.show(1, "foo");
        
        ASSERT_EQ(observed_output, "S[1,\"foo\"]");
        ASSERT_EQ(success, true);
}

TEST_F(crystaldisplay_tests, clear_produces_a_valid_command)
{
        set_reply("[0]");
        EXPECT_CALL(romi_serial, send(_,_))
                .WillOnce(Invoke(this, &crystaldisplay_tests::set_output))
                .RetiresOnSaturation();

        CrystalDisplay display(romi_serial);
        bool success = display.clear(1);
        
        ASSERT_EQ(observed_output, "C[1]");
        ASSERT_EQ(success, true);
}

TEST_F(crystaldisplay_tests, show_returns_false_on_firmware_error)
{
        set_reply("[1,\"message\"]");
        EXPECT_CALL(romi_serial, send(_,_))
                .WillOnce(Invoke(this, &crystaldisplay_tests::set_output))
                .RetiresOnSaturation();

        CrystalDisplay display(romi_serial);
        bool success = display.show(1, "foo");
        
        ASSERT_EQ(observed_output, "S[1,\"foo\"]");
        ASSERT_EQ(success, false);
}

TEST_F(crystaldisplay_tests, clear_returns_false_on_firmware_error)
{
        set_reply("[1,\"message\"]");
        EXPECT_CALL(romi_serial, send(_,_))
                .WillOnce(Invoke(this, &crystaldisplay_tests::set_output))
                .RetiresOnSaturation();

        CrystalDisplay display(romi_serial);
        bool success = display.clear(1);
        
        ASSERT_EQ(observed_output, "C[1]");
        ASSERT_EQ(success, false);
}
