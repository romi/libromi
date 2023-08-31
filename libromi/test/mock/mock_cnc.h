#include "gmock/gmock.h"
#include "api/ICNC.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockCNC : public romi::ICNC
{
public:
        MOCK_METHOD(bool, get_range, (romi::CNCRange &range), (override));
        MOCK_METHOD(bool, moveto, (double x, double y, double z, double relative_speed), (override));
        //MOCK_METHOD(bool, moveat, (int16_t speed_x, int16_t speed_y, int16_t speed_z), (override));
        MOCK_METHOD(bool, spindle, (double speed), (override));
        MOCK_METHOD(bool, travel, (romi::Path &path, double relative_speed), (override));
        MOCK_METHOD(uint8_t, count_relays, (), (override));
        MOCK_METHOD(bool, set_relay, (uint8_t, bool), (override));
        MOCK_METHOD(bool, homing, (), (override));
        MOCK_METHOD(bool, pause_activity, (), (override));
        MOCK_METHOD(bool, continue_activity, (), (override));
        MOCK_METHOD(bool, reset_activity, (), (override));
        MOCK_METHOD(bool, power_up, (), (override));
        MOCK_METHOD(bool, power_down, (), (override));
        MOCK_METHOD(bool, stand_by, (), (override));
        MOCK_METHOD(bool, wake_up, (), (override));
        MOCK_METHOD(bool, get_position, (romi::v3& position), (override));
        MOCK_METHOD(bool, synchronize, (double timeout_seconds), (override));
        MOCK_METHOD(bool, helix, (double xc, double yc, double alpha, double dz, double relative_speed), (override)); 
};
#pragma GCC diagnostic pop
