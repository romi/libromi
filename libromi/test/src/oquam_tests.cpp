#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock_cnccontroller.h"
#include "mock_clock.h"
#include "oquam/Oquam.h"

#include <ClockAccessor.h>
#include "Linux.h"
#include "data_provider/RomiDeviceData.h"
#include "data_provider/SoftwareVersion.h"
#include "session/Session.h"
#include "data_provider/Gps.h"
#include "data_provider/GpsLocationProvider.h"

using namespace std;
using namespace testing;
using namespace romi;

class oquam_tests : public ::testing::Test
{
protected:
        
        int32_t position[3] =  {0, 0, 0};
        
        const double xmin[3] =  {0, 0, 0};
        const double xmax[3] =  {0.5, 0.5, 0};
        const double vmax[3] =  {0.1, 0.1, 0.01};
        const double amax[3] =  {0.2, 0.2, 0.2};
        const double scale[3] = {40000, 40000, 100000};
        const double slice_interval = 0.020;
        const double max_slice_interval = 4.0;
        const romi::AxisIndex homing[3] = {romi::kAxisX, romi::kAxisY, romi::kNoAxis};
        
        CNCRange range;
        OquamSettings settings;
        MockCNCController controller;
        
	oquam_tests()
                : range(xmin, xmax),
                  settings(range, vmax, amax, scale, 0.005,
                           slice_interval, max_slice_interval, homing),
                  controller(),
                  linux(),
                  romiDeviceData(),
                  softwareVersion(),
                  gps(),
                  locationPrivider(),
                  session_directory("./session-directory"),
                  observation_id("observation_id"),
                  mockClock_(std::make_shared<rpp::MockClock>()) {
                locationPrivider  = std::make_unique<GpsLocationProvider>(gps);
        }

	~oquam_tests() override = default;

	void SetUp() override {
                position[0] = 0;
                position[1] = 0;
                position[2] = 0;
                rpp::ClockAccessor::SetInstance(mockClock_);
                std::string date_time("01012025");
                EXPECT_CALL(*mockClock_, datetime_compact_string)
                                .Times(AtLeast(1))
                                .WillRepeatedly(Return(date_time));
        }

	void TearDown() override {
                rpp::ClockAccessor::SetInstance(nullptr);
	}

        void DefaultSetUp() {
                EXPECT_CALL(controller, get_position(NotNull()))
                        .WillRepeatedly(DoAll(SetArrayArgument<0>(position, position+3),
                                              Return(true)));
                EXPECT_CALL(controller, configure_homing(_,_,_))
                        .WillRepeatedly(Return(true));
                EXPECT_CALL(controller, enable())
                        .WillRepeatedly(Return(true));
                EXPECT_CALL(controller, homing())
                        .WillRepeatedly(Return(true));
                EXPECT_CALL(controller, synchronize(_))
                        .WillRepeatedly(Return(true));
                EXPECT_CALL(controller, move(_,_,_,_))
                        .WillRepeatedly(Return(true));
        }
        
public:
        
        bool get_position(int32_t *p) {
                for (int i = 0; i < 3; i++)
                        p[i] = position[i];
                return true;
        }
        
        bool move(int16_t millis, int16_t x, int16_t y, int16_t z) {
                r_debug("move(dt=%d, dx=%d, dy=%d, dz=%d)", millis, x, y, z);
                position[0] += x;
                position[1] += y;
                position[2] += z;
                return true;
        }

        rpp::Linux linux;
        RomiDeviceData romiDeviceData;
        SoftwareVersion softwareVersion;
        romi::Gps gps;
        std::unique_ptr<ILocationProvider> locationPrivider;
        const std::string session_directory;
        const std::string observation_id;
        std::shared_ptr<rpp::MockClock> mockClock_;
};

TEST_F(oquam_tests, constructor_calls_configure_homing)
{
        EXPECT_CALL(controller, configure_homing(_,_,_))
                .Times(1)
                .WillOnce(Return(true));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);

        Oquam oquam(controller, settings, session);
}

TEST_F(oquam_tests, constructor_throws_exception_when_configure_homing_fails)
{
        EXPECT_CALL(controller, configure_homing(_,_,_))
                .Times(1)
                .WillOnce(Return(false));
        
        try {
                romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
                session.start(observation_id);
                Oquam oquam(controller, settings, session);
                FAIL() << "Excpected a runtime error";
                
        } catch (std::runtime_error& e) {
                // OK
        }
}

TEST_F(oquam_tests, pause_activity_calls_controller_1)
{
        DefaultSetUp();
        EXPECT_CALL(controller, pause_activity())
                .Times(1)
                .WillOnce(Return(true));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        bool success = oquam.pause_activity();
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, pause_activity_calls_controller_2)
{
        DefaultSetUp();
        EXPECT_CALL(controller, pause_activity())
                .Times(1)
                .WillOnce(Return(false));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        bool success = oquam.pause_activity();
        ASSERT_EQ(success, false);
}

TEST_F(oquam_tests, continue_activity_calls_controller_1)
{
        DefaultSetUp();
        EXPECT_CALL(controller, continue_activity())
                .Times(1)
                .WillOnce(Return(true));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        bool success = oquam.continue_activity();
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, continue_activity_calls_controller_2)
{
        DefaultSetUp();
        EXPECT_CALL(controller, continue_activity())
                .Times(1)
                .WillOnce(Return(false));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        bool success = oquam.continue_activity();
        ASSERT_EQ(success, false);
}

TEST_F(oquam_tests, reset_calls_controller_1)
{
        DefaultSetUp();
        EXPECT_CALL(controller, reset_activity())
                .Times(1)
                .WillOnce(Return(true));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        bool success = oquam.reset_activity();
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, reset_calls_controller_2)
{
        DefaultSetUp();
        EXPECT_CALL(controller, reset_activity())
                .Times(1)
                .WillOnce(Return(false));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        bool success = oquam.reset_activity();
        ASSERT_EQ(success, false);
}

TEST_F(oquam_tests, constructor_copies_range)
{
        EXPECT_CALL(controller, configure_homing(_,_,_))
                .Times(1)
                .WillOnce(Return(true));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);

        CNCRange range;
        oquam.get_range(range);
        ASSERT_EQ(range.min.x(), xmin[0]);
        ASSERT_EQ(range.max.x(), xmax[0]);
        ASSERT_EQ(range.min.y(), xmin[1]);
        ASSERT_EQ(range.max.y(), xmax[1]);
        ASSERT_EQ(range.min.z(), xmin[2]);
        ASSERT_EQ(range.max.z(), xmax[2]);
}

TEST_F(oquam_tests, moveto_returns_error_when_speed_is_invalid)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        
        ASSERT_EQ(oquam.moveto(0.1, 0.0, 0.0, 1.1), false);
        ASSERT_EQ(oquam.moveto(0.1, 0.0, 0.0, -0.1), false);
}

TEST_F(oquam_tests, moveto_returns_error_when_position_is_invalid)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        ASSERT_EQ(oquam.moveto(range.min.x()-0.1, 0.0, 0.0, 0.1), false);
        ASSERT_EQ(oquam.moveto(range.max.x()+0.1, 0.0, 0.0, 0.1), false);
        ASSERT_EQ(oquam.moveto(0.0, range.min.y()-0.1, 0.0, 0.1), false);
        ASSERT_EQ(oquam.moveto(0.0, range.max.y()+0.1, 0.0, 0.1), false);
        ASSERT_EQ(oquam.moveto(0.0, 0.0, range.min.z()-0.1, 0.1), false);
        ASSERT_EQ(oquam.moveto(0.0, 0.0, range.max.z()+0.1, 0.1), false);
}

TEST_F(oquam_tests, returns_false_when_get_position_fails)
{
        EXPECT_CALL(controller, configure_homing(_,_,_))
                .Times(1)
                .WillOnce(Return(true));
        EXPECT_CALL(controller, get_position(_))
                .Times(1)
                .WillOnce(Return(false));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        bool success = oquam.moveto(0.1, 0.0, 0.0, 0.3);
        ASSERT_EQ(success, false);        
}

TEST_F(oquam_tests, returns_false_when_moveto_fails)
{
        EXPECT_CALL(controller, configure_homing(_,_,_))
                .Times(1)
                .WillOnce(Return(true));
        EXPECT_CALL(controller, get_position(_))
                .WillRepeatedly(Invoke(this, &oquam_tests::get_position));
        EXPECT_CALL(controller, move(_,_,_,_))
                .Times(1)
                .WillOnce(Return(false));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        bool success = oquam.moveto(0.1, 0.0, 0.0, 0.3);
        ASSERT_EQ(success, false);        
}

TEST_F(oquam_tests, returns_false_when_synchronize_fails)
{
        EXPECT_CALL(controller, configure_homing(_,_,_))
                .Times(1)
                .WillOnce(Return(true));
        EXPECT_CALL(controller, get_position(_))
                .WillRepeatedly(Invoke(this, &oquam_tests::get_position));
        EXPECT_CALL(controller, move(_,_,_,_))
                .WillRepeatedly(Return(true));
        EXPECT_CALL(controller, synchronize(_))
                .Times(1)
                .WillOnce(Return(false));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        bool success = oquam.moveto(0.1, 0.0, 0.0, 0.3);
        ASSERT_EQ(success, false);        
}

TEST_F(oquam_tests, test_oquam_moveto)
{
        InSequence seq;

        EXPECT_CALL(controller, configure_homing(_,_,_))
                .Times(1)
                .WillOnce(Return(true));
        EXPECT_CALL(controller, get_position(_))
                .WillRepeatedly(Invoke(this, &oquam_tests::get_position));
        EXPECT_CALL(controller, move(_,_,_,_))
                .WillRepeatedly(Invoke(this, &oquam_tests::move));
        EXPECT_CALL(controller, synchronize(_))
                .Times(1)
                .WillOnce(Return(true));

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);
        bool success = oquam.moveto(0.1, 0.0, 0.0, 0.3);
        ASSERT_EQ(success, true);
        ASSERT_EQ(position[0], 4000);        
}

TEST_F(oquam_tests, test_oquam_moveto_2)
{
        InSequence seq;

        EXPECT_CALL(controller, configure_homing(_,_,_))
                .Times(1)
                .WillOnce(Return(true));
        for (int i = 0; i < 2; i++) {
                EXPECT_CALL(controller, get_position(_))
                        .WillRepeatedly(Invoke(this, &oquam_tests::get_position));
                EXPECT_CALL(controller, move(_,_,_,_))
                        .WillRepeatedly(Invoke(this, &oquam_tests::move));
                EXPECT_CALL(controller, synchronize(_))
                        .Times(1)
                        .WillOnce(Return(true));
        }

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start(observation_id);
        Oquam oquam(controller, settings, session);

        bool success = oquam.moveto(0.1, 0.0, 0.0, 0.3);
        ASSERT_EQ(success, true);
        
        success = oquam.moveto(0.0, 0.0, 0.0, 0.3);
        ASSERT_EQ(success, true);
        
        ASSERT_EQ(position[0], 0);        
}

TEST_F(oquam_tests, test_oquam_travel_empty_path)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start("travel_empty");
        Oquam oquam(controller, settings, session);

        Path path;
        bool success = oquam.travel(path, 0.3);
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, test_oquam_travel_square)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start("travel_square");
        Oquam oquam(controller, settings, session);

        Path path;
        v3 p0(0.1, 0.0, 0.0);
        v3 p1(0.1, 0.1, 0.0);
        v3 p2(0.0, 0.1, 0.0);
        v3 p3(0.0, 0.0, 0.0);
        path.push_back(p0);
        path.push_back(p1);
        path.push_back(p2);
        path.push_back(p3);

        bool success = oquam.travel(path, 0.3);
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, test_oquam_travel_square_fast)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start("travel_fast");
        Oquam oquam(controller, settings, session);

        Path path;
        v3 p0(0.1, 0.0, 0.0);
        v3 p1(0.1, 0.1, 0.0);
        v3 p2(0.0, 0.1, 0.0);
        v3 p3(0.0, 0.0, 0.0);
        path.push_back(p0);
        path.push_back(p1);
        path.push_back(p2);
        path.push_back(p3);

        bool success = oquam.travel(path, 1.0);
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, test_oquam_travel_snake)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start("travel_snake");
        Oquam oquam(controller, settings, session);

        Path path;
        int N = 10;
        for (int i = 1; i <= N; i++) {
                v3 p0(i * 0.01, (i-1) * 0.01, 0.0);
                path.push_back(p0);
                v3 p1(i * 0.01, i * 0.01, 0.0);
                path.push_back(p1);
        }
        
        v3 p(0.0, 0.0, 0.0);
        path.push_back(p);

        bool success = oquam.travel(path, 1.0);
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, test_oquam_travel_snake_2)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start("travel_snake_2");
        Oquam oquam(controller, settings, session);

        int N = 11;
        Path path;
        double x = 0.0;
        double y = 0.0;
        for (int i = 1; i <= N; i++) {
                int n = N + 1 - i;
                double len = 0.001 * n;
                v3 p0(x + len, y, 0.0);
                path.push_back(p0);
                v3 p1(x + len, y + len, 0.0);
                path.push_back(p1);
                x += len;
                y += len;
        }
        
        v3 p(0.0, 0.0, 0.0);
        path.push_back(p);

        bool success = oquam.travel(path, 1.0);
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, test_oquam_travel_round_trip)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start("travel_round_trip");
        Oquam oquam(controller, settings, session);

        Path path;
        path.push_back(v3(0.0, 0.0, 0.0));
        path.push_back(v3(0.1, 0.0, 0.0));
        path.push_back(v3(0.0, 0.0, 0.0));
        
        bool success = oquam.travel(path, 1.0);
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, test_oquam_travel_collinear)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start("travel_round_collinear");
        Oquam oquam(controller, settings, session);

        Path path;
        path.push_back(v3(0.0, 0.0, 0.0));
        path.push_back(v3(0.1, 0.0, 0.0));
        path.push_back(v3(0.2, 0.0, 0.0));
        path.push_back(v3(0.0, 0.0, 0.0));
        
        bool success = oquam.travel(path, 1.0);
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, test_oquam_travel_large_displacement)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start("travel_large_displacement");
        Oquam oquam(controller, settings, session);

        Path path;
        path.push_back(v3(0.0, 0.0, 0.0));
        path.push_back(v3(0.1, 0.0, 0.0));
        path.push_back(v3(0.1, 0.07, 0.0));
        path.push_back(v3(0.2, 0.07, 0.0));
        path.push_back(v3(0.0, 0.0, 0.0));
        
        bool success = oquam.travel(path, 1.0);
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, test_oquam_travel_small_displacement)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start("travel_small_displacement");
        Oquam oquam(controller, settings, session);

        Path path;
        path.push_back(v3(0.0, 0.0, 0.0));
        path.push_back(v3(0.1, 0.0, 0.0));
        path.push_back(v3(0.1, 0.04, 0.0));
        path.push_back(v3(0.2, 0.04, 0.0));
        path.push_back(v3(0.0, 0.0, 0.0));
        
        bool success = oquam.travel(path, 1.0);
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, test_oquam_travel_tiny_displacement)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData, softwareVersion, std::move(locationPrivider));
        session.start("travel_tiny_displacement");
        Oquam oquam(controller, settings, session);
        //oquam.set_file_cabinet(&debug_session);

        Path path;
        path.push_back(v3(0.0, 0.0, 0.0));
        path.push_back(v3(0.1, 0.0, 0.0));
        path.push_back(v3(0.1, 0.005, 0.0));
        path.push_back(v3(0.2, 0.005, 0.0));
        path.push_back(v3(0.0, 0.0, 0.0));
        
        bool success = oquam.travel(path, 1.0);
        ASSERT_EQ(success, true);
}

TEST_F(oquam_tests, test_oquam_travel_zigzag)
{
        DefaultSetUp();

        romi::Session session(linux, session_directory, romiDeviceData,
                              softwareVersion, std::move(locationPrivider));
        session.start("travel_zigzag");
        Oquam oquam(controller, settings, session);

        Path path;
        v3 p(0.0, 0.0, 0.0);
        
        for (int i = 1; i <= 3; i++) {
                p.y() += 0.01;
                path.push_back(p);
                
                p.x() += 0.1;
                path.push_back(p);
                
                p.y() += 0.01;
                path.push_back(p);

                p.x() -= 0.1;
                path.push_back(p);
        }
        
        path.push_back(v3(0.0, 0.0, 0.0));

        bool success = oquam.travel(path, 1.0);
        ASSERT_EQ(success, true);
}


