#include "gmock/gmock.h"
#include "api/IWeeder.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockWeeder : public romi::IWeeder
{
public:
        MOCK_METHOD(bool, hoe, (), (override));
        MOCK_METHOD(bool, stop, (), (override));
        MOCK_METHOD(bool, pause_activity, (), (override));
        MOCK_METHOD(bool, continue_activity, (), (override));
        MOCK_METHOD(bool, reset_activity, (), (override));
        MOCK_METHOD(bool, power_up, (), (override));
        MOCK_METHOD(bool, power_down, (), (override));
        MOCK_METHOD(bool, stand_by, (), (override));
        MOCK_METHOD(bool, wake_up, (), (override));
};
#pragma GCC diagnostic pop