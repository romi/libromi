#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "JsonCpp.h"
#include "rover/Navigation.h"
#include "mock_motordriver.h"
#include "mock_trackfollower.h"
#include "mock_navigationcontroller.h"
#include "mock_distancemeasure.h"
#include "mock_session.h"

using namespace std;
using namespace testing;
using namespace romi;

class navigation_tests : public ::testing::Test
{
protected:
        JsonCpp config;
        MockMotorDriver driver;
        MockDistanceMeasure distance_measure;
        MockTrackFollower track_follower;
        MockNavigationController navigation_controller;
        MockSession session;
        
	navigation_tests()
                : config(),
                  driver(),
                  distance_measure(),
                  track_follower(),
                  navigation_controller(),
                  session() {

                const char * config_string = "{"
                        "'wheel-diameter': 1.0,"
                        "'wheel-base': 1.0,"
                        "'encoder-steps': 1000.0,"
                        "'maximum-speed': 3.0, "
                        "'maximum-acceleration': 0.1 }";
                config = JsonCpp::parse(config_string);
	}

	~navigation_tests() override = default;

	void SetUp() override {
                EXPECT_CALL(driver, get_pid_values(_,_,_,_,_,_,_,_))
                        .WillRepeatedly(Return(true));
	}

	void TearDown() override {
	}
};

// TEST_F(navigation_tests, test_moveat)
// {
//         NavigationSettings settings(config);
//         Navigation navigation(driver, settings, track_follower, session);

//         EXPECT_CALL(driver, moveat(0.1, 0.2))
//                 .WillOnce(Return(true));

//         navigation.moveat(0.1, 0.2);
// }

TEST_F(navigation_tests, move_fails_with_invalid_speed_1)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);

        EXPECT_CALL(driver, stop())
                .WillOnce(Return(true));

        bool success = navigation.move(1.0, 2.0);
        ASSERT_EQ(success, false);
}

TEST_F(navigation_tests, move_fails_with_invalid_speed_2)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);

        EXPECT_CALL(driver, stop())
                .WillOnce(Return(true));

        bool success = navigation.move(1.0, -2.0);
        ASSERT_EQ(success, false);
}

TEST_F(navigation_tests, move_fails_with_invalid_distance_1)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);

        EXPECT_CALL(driver, stop())
                .WillOnce(Return(true));

        bool success = navigation.move(51.0, 1.0);
        ASSERT_EQ(success, false);
}

TEST_F(navigation_tests, move_fails_with_invalid_distance_2)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);

        EXPECT_CALL(driver, stop())
                .WillOnce(Return(true));

        bool success = navigation.move(-51.0, 1.0);
        ASSERT_EQ(success, false);
}

// TEST_F(navigation_tests, successful_move)
// {
//         NavigationSettings settings(config);
//         Navigation navigation(settings, driver, distance_measure, track_follower,
//                              navigation_controller, session);
                
//         EXPECT_CALL(driver, stop())
//                 .WillRepeatedly(Return(true));
//         EXPECT_CALL(driver, moveat(0,0))
//                 .WillRepeatedly(Return(true));

//         {
//                 InSequence seq;
//                 EXPECT_CALL(driver, get_encoder_values(_,_,_))
//                         .WillOnce(DoAll(SetArgReferee<0>(0.0),
//                                         SetArgReferee<1>(0.0),
//                                         SetArgReferee<2>(0.0),
//                                         Return(true)))
//                         .RetiresOnSaturation();
                
//                 EXPECT_CALL(driver, moveat(1.0, 1.0))
//                         .WillOnce(Return(true))
//                         .RetiresOnSaturation();
                
//                 EXPECT_CALL(driver, get_encoder_values(_,_,_))
//                         .WillOnce(DoAll(SetArgReferee<0>(3.0 * 1000.0 / M_PI),
//                                         SetArgReferee<1>(3.0 * 1000.0 / M_PI),
//                                         SetArgReferee<2>(1.0),
//                                         Return(true)))
//                         .RetiresOnSaturation();

//                 // EXPECT_CALL(driver, moveat(0.0, 0.0))
//                 //         .WillOnce(Return(true))
//                 //         .RetiresOnSaturation();
//         }

//         // Move 3 meter, at the maximum speed of 3 m/s.
//         bool success = navigation.move(3.0, 1.0);
//         ASSERT_EQ(success, true);
// }

// TEST_F(navigation_tests, successfully_move_distance_with_negative_speed)
// {
//         NavigationSettings settings(config);
//         Navigation navigation(settings, driver, distance_measure, track_follower,
//                              navigation_controller, session);
                
//         EXPECT_CALL(driver, stop())
//                 .WillRepeatedly(Return(true));

//         {
//                 InSequence seq;
                
//                 EXPECT_CALL(driver, get_encoder_values(_,_,_))
//                         .WillOnce(DoAll(SetArgReferee<0>(0.0),
//                                         SetArgReferee<1>(0.0),
//                                         SetArgReferee<2>(0.0),
//                                         Return(true)))
//                         .RetiresOnSaturation();
                
//                 EXPECT_CALL(driver, moveat(-1.0, -1.0))
//                         .WillOnce(Return(true))
//                         .RetiresOnSaturation();
                
//                 EXPECT_CALL(driver, get_encoder_values(_,_,_))
//                         .WillOnce(DoAll(SetArgReferee<0>(-3.0 * 1000.0 / M_PI),
//                                         SetArgReferee<1>(-3.0 * 1000.0 / M_PI),
//                                         SetArgReferee<2>(1.0),
//                                         Return(true)))
//                         .RetiresOnSaturation();
//         }

//         // Move 3 meter, at the maximum speed of 3 m/s.
//         bool success = navigation.move(3.0, -1.0);
//         ASSERT_EQ(success, true);
// }

// TEST_F(navigation_tests, successfully_move_negative_distance_with_positive_speed)
// {
//         NavigationSettings settings(config);
//         Navigation navigation(settings, driver, distance_measure, track_follower,
//                              navigation_controller, session);
        
//         EXPECT_CALL(driver, stop())
//                 .WillRepeatedly(Return(true));

//         {
//                 InSequence seq;
                
//                 EXPECT_CALL(driver, get_encoder_values(_,_,_))
//                         .WillOnce(DoAll(SetArgReferee<0>(0.0),
//                                         SetArgReferee<1>(0.0),
//                                         SetArgReferee<2>(0.0),
//                                         Return(true)))
//                         .RetiresOnSaturation();
                
//                 EXPECT_CALL(driver, moveat(-1.0, -1.0))
//                         .WillOnce(Return(true))
//                         .RetiresOnSaturation();
                
//                 EXPECT_CALL(driver, get_encoder_values(_,_,_))
//                         .WillOnce(DoAll(SetArgReferee<0>(-3.0 * 1000.0 / M_PI),
//                                         SetArgReferee<1>(-3.0 * 1000.0 / M_PI),
//                                         SetArgReferee<2>(1.0),
//                                         Return(true)))
//                         .RetiresOnSaturation();
//         }

//         // Move 3 meter, at the maximum speed of 3 m/s.
//         bool success = navigation.move(-3.0, 1.0);
//         ASSERT_EQ(success, true);
// }

// TEST_F(navigation_tests, successfully_move_negative_distance_with_negative_speed)
// {
//         NavigationSettings settings(config);
//         Navigation navigation(settings, driver, distance_measure, track_follower,
//                              navigation_controller, session);

//         EXPECT_CALL(driver, stop())
//                 .WillRepeatedly(Return(true));
        
//         {
//                 InSequence seq;
                
//                 EXPECT_CALL(driver, get_encoder_values(_,_,_))
//                         .WillOnce(DoAll(SetArgReferee<0>(0.0),
//                                         SetArgReferee<1>(0.0),
//                                         SetArgReferee<2>(0.0),
//                                         Return(true)))
//                         .RetiresOnSaturation();
                
//                 EXPECT_CALL(driver, moveat(-1.0, -1.0))
//                         .WillOnce(Return(true))
//                         .RetiresOnSaturation();
                
//                 EXPECT_CALL(driver, get_encoder_values(_,_,_))
//                         .WillOnce(DoAll(SetArgReferee<0>(-3.0 * 1000.0 / M_PI),
//                                         SetArgReferee<1>(-3.0 * 1000.0 / M_PI),
//                                         SetArgReferee<2>(1.0),
//                                         Return(true)))
//                         .RetiresOnSaturation();
//         }

//         // Move 3 meter, at the maximum speed of 3 m/s.
//         bool success = navigation.move(-3.0, -1.0);
//         ASSERT_EQ(success, true);
// }

TEST_F(navigation_tests, move_fails_on_zero_speed)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);
        
        EXPECT_CALL(driver, stop())
                .WillOnce(Return(true))
                .RetiresOnSaturation();
        
        bool success = navigation.move(3.0, 0.0);
        ASSERT_EQ(success, false);
}

TEST_F(navigation_tests, move_returns_true_on_zero_distance)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);
        
        EXPECT_CALL(driver, stop())
                .WillOnce(Return(true))
                .RetiresOnSaturation();
        
        bool success = navigation.move(0.0, 1.0);
        ASSERT_EQ(success, true);
}

TEST_F(navigation_tests, move_fails_on_bad_speed_1)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);
        
        EXPECT_CALL(driver, stop())
                .WillOnce(Return(true))
                .RetiresOnSaturation();
        
        bool success = navigation.move(3.0, 2.0);
        ASSERT_EQ(success, false);
}

TEST_F(navigation_tests, move_fails_on_bad_speed_2)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);
        
        EXPECT_CALL(driver, stop())
                .WillOnce(Return(true))
                .RetiresOnSaturation();
        
        bool success = navigation.move(3.0, -1.1);
        ASSERT_EQ(success, false);
}

TEST_F(navigation_tests, move_fails_on_bad_speed_3)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);
        
        EXPECT_CALL(driver, stop())
                .WillOnce(Return(true))
                .RetiresOnSaturation();
        
        bool success = navigation.move(3.0, 0);
        ASSERT_EQ(success, false);
}

TEST_F(navigation_tests, move_returns_false_on_failing_stop)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);
        
        EXPECT_CALL(driver, stop())
                .WillOnce(Return(false))
                .RetiresOnSaturation();
        
        EXPECT_CALL(driver, stop())
                .WillOnce(Return(false))
                .RetiresOnSaturation();
        
        bool success = navigation.move(3.0, 0.5);
        ASSERT_EQ(success, false);
}

// TEST_F(navigation_tests, move_returns_false_on_failing_get_encoders)
// {
//         NavigationSettings settings(config);
//         Navigation navigation(settings, driver, distance_measure, track_follower,
//                               navigation_controller, session);

//         EXPECT_CALL(driver, stop())
//                 .WillRepeatedly(Return(true));
                
//         EXPECT_CALL(driver, get_encoder_values(_,_,_))
//                 .WillOnce(Return(false))
//                 .RetiresOnSaturation();
        
//         bool success = navigation.move(3.0, 0.5);
//         ASSERT_EQ(success, false);
// }

// TEST_F(navigation_tests, move_returns_false_when_moveat_fails)
// {
//         NavigationSettings settings(config);
//         Navigation navigation(settings, driver, distance_measure, track_follower,
//                              navigation_controller, session);

//         EXPECT_CALL(driver, stop())
//                 .WillRepeatedly(Return(true));

//         {
//                 InSequence seq;
                
//                 EXPECT_CALL(driver, get_encoder_values(_,_,_))
//                         .WillOnce(DoAll(SetArgReferee<0>(0.0),
//                                         SetArgReferee<1>(0.0),
//                                         SetArgReferee<2>(0.0),
//                                         Return(true)))
//                         .RetiresOnSaturation();
                
//                 EXPECT_CALL(driver, moveat(1.0, 1.0))
//                         .WillOnce(Return(false))
//                         .RetiresOnSaturation();
//         }

//         // Move 3 meter, at the maximum speed of 3 m/s.
//         bool success = navigation.move(3.0, 1.0);
//         ASSERT_EQ(success, false);
// }

TEST_F(navigation_tests, stop_succeeds)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);
        
        EXPECT_CALL(driver, stop())
                .WillOnce(Return(true))
                .RetiresOnSaturation();

        bool success = navigation.stop();
        ASSERT_EQ(success, true);
}

TEST_F(navigation_tests, stop_fails_when_stop_driver_fails)
{
        NavigationSettings settings(config);
        Navigation navigation(settings, driver, distance_measure, track_follower,
                              navigation_controller, session);
        
        EXPECT_CALL(driver, stop())
                .WillOnce(Return(false))
                .RetiresOnSaturation();

        bool success = navigation.stop();
        ASSERT_EQ(success, false);
}

TEST_F(navigation_tests, initialise_initialises_steering)
{
    NavigationSettings settings(config);
    Navigation navigation(settings, driver, distance_measure, track_follower,
                          navigation_controller, session);

    EXPECT_CALL(navigation_controller, initialise())
        .WillOnce(Return(true));

    bool success = navigation.stop();
    ASSERT_EQ(success, false);
}