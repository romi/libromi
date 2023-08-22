#include "gmock/gmock.h"
#include "oquam/ICNCController.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockCNCController : public romi::ICNCController
{
public:
        MOCK_METHOD(bool, get_position, (int32_t *pos), (override));
        MOCK_METHOD(bool, homing, (), (override));
        MOCK_METHOD(bool, spindle, (double speed), (override));
        MOCK_METHOD(bool, synchronize, (double timeout), (override));
        MOCK_METHOD(bool, move, (int16_t millis, int16_t steps_x, int16_t steps_y, int16_t steps_z), (override));
        MOCK_METHOD(bool, moveat, (int16_t speed_x, int16_t speed_y, int16_t speed_z), (override));
        MOCK_METHOD(bool, moveto, (int16_t millis, int16_t x, int16_t y, int16_t z), (override));
        MOCK_METHOD(bool, pause_activity, (), (override));
        MOCK_METHOD(bool, continue_activity, (), (override));
        MOCK_METHOD(bool, reset_activity, (), (override));
        MOCK_METHOD(bool, enable, (), (override));
        MOCK_METHOD(bool, disable, (), (override));
        MOCK_METHOD(bool, set_homing_axes, (romi::AxisIndex axis1, romi::AxisIndex axis2, romi::AxisIndex axis3), (override));
        MOCK_METHOD(bool, set_homing_speeds, (int16_t axis1, int16_t axis2, int16_t axis3), (override));
        MOCK_METHOD(bool, set_homing_mode, (romi::HomingMode mode), (override));
        MOCK_METHOD(bool, stop, (), (override));
};
#pragma GCC diagnostic pop
