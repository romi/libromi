#include "gmock/gmock.h"
#include "api/Joystick.h"

class MockJoystick : public romi::Joystick
{
public:
        MOCK_METHOD(romi::JoystickEvent&, get_next_event, (), (override));
        MOCK_METHOD(size_t, get_num_axes, (), (override));
        MOCK_METHOD(double, get_axis, (size_t i), (override));
        MOCK_METHOD(size_t, get_num_buttons, (), (override));
        MOCK_METHOD(bool, is_button_pressed, (size_t i), (override));
};
