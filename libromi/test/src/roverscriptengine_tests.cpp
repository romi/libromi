#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock_inputdevice.h"
#include "mock_display.h"
#include "mock_speedcontroller.h"
#include "mock_navigation.h"
#include "mock_eventtimer.h"
#include "mock_menu.h"
#include "mock_scriptengine.h"
#include "mock_notifications.h"
#include "mock_weeder.h"
#include "rover/Rover.h"
#include "rover/RoverScriptEngine.h"
#include "ui/ScriptList.h"
#include <ClockAccessor.h>

using namespace std;
using namespace testing;


class roverscriptengine_tests : public ::testing::Test
{
protected:
        MockInputDevice input_device;
        MockDisplay display;
        MockSpeedController speed_controller;
        MockNavigation navigation;
        MockEventTimer event_timer;
        MockMenu menu;
        MockNotifications notifications;
        MockWeeder weeder;
        JsonCpp json;

        
	roverscriptengine_tests() : input_device(), display(), speed_controller(), navigation(), event_timer(),
	                                menu(), notifications(), weeder(), json(){
                json = JsonCpp::parse("[{ 'id': 'foo', 'title': 'Foo', "
                                      "  'actions': [{'action':'move', 'distance': 1, 'speed': 0.5}]},"
                                      "{ 'id': 'bar', 'title': 'Bar', "
                                      "  'actions': [{'action':'hoe'}]}]");
	}

	~roverscriptengine_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}
};

TEST_F(roverscriptengine_tests, test_constructor)
{
        romi::ScriptList scripts(json);
        romi::RoverScriptEngine script_engine(scripts, 1, 2);
}

TEST_F(roverscriptengine_tests, get_next_event_returns_none)
{
        romi::ScriptList scripts(json);
        romi::RoverScriptEngine script_engine(scripts, 1, 2);

        ASSERT_EQ(0, script_engine.get_next_event());
}

TEST_F(roverscriptengine_tests, calls_move)
{
        romi::ScriptList scripts(json);
        romi::RoverScriptEngine script_engine(scripts, 1, 2);

        romi::Rover rover(input_device, display,
                    speed_controller,
                    navigation,
                    event_timer,
                    menu,
                    script_engine,
                    notifications,
                    weeder);

        EXPECT_CALL(navigation, move(1.0, 0.5))
                .WillOnce(Return(true));
        
        script_engine.execute_script(rover, 0);
        rpp::ClockAccessor::GetInstance()->sleep(0.050); // OK?
        
        ASSERT_EQ(1, script_engine.get_next_event());
}

TEST_F(roverscriptengine_tests, calls_hoe)
{
        romi::ScriptList scripts(json);
        romi::RoverScriptEngine script_engine(scripts, 1, 2);

        romi::Rover rover(input_device, display,
                    speed_controller,
                    navigation,
                    event_timer,
                    menu,
                    script_engine,
                    notifications,
                    weeder);

        EXPECT_CALL(weeder, hoe())
                .WillOnce(Return(true));
        
        script_engine.execute_script(rover, 1);
        rpp::ClockAccessor::GetInstance()->sleep(0.050); // OK?
        
        ASSERT_EQ(1, script_engine.get_next_event());
}

TEST_F(roverscriptengine_tests, returns_error_event_on_failed_script_1)
{
        romi::ScriptList scripts(json);
        romi::RoverScriptEngine script_engine(scripts, 1, 2);

        romi::Rover rover(input_device, display,
                    speed_controller,
                    navigation,
                    event_timer,
                    menu,
                    script_engine,
                    notifications,
                    weeder);

        EXPECT_CALL(navigation, move(1.0, 0.5))
                .WillOnce(Return(false));
        
        script_engine.execute_script(rover, 0);
        rpp::ClockAccessor::GetInstance()->sleep(0.050); // OK?
        
        ASSERT_EQ(2, script_engine.get_next_event());
}

TEST_F(roverscriptengine_tests, returns_error_event_on_failed_script_2)
{
        romi::ScriptList scripts(json);
        romi::RoverScriptEngine script_engine(scripts, 1, 2);

        romi::Rover rover(input_device, display,
                    speed_controller,
                    navigation,
                    event_timer,
                    menu,
                    script_engine,
                    notifications,
                    weeder);

        EXPECT_CALL(weeder, hoe())
                .WillOnce(Return(false));
        
        script_engine.execute_script(rover, 1);
        rpp::ClockAccessor::GetInstance()->sleep(0.050); // OK?
        
        ASSERT_EQ(2, script_engine.get_next_event());
}
