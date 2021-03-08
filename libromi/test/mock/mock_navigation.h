#include "gmock/gmock.h"
#include "api/INavigation.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockNavigation : public romi::INavigation
{
public:
        MOCK_METHOD(bool, moveat, (double left, double right), (override));
        MOCK_METHOD(bool, move, (double distance, double speed), (override));
        MOCK_METHOD(bool, stop, (), (override));
        MOCK_METHOD(bool, pause_activity, (), (override));
        MOCK_METHOD(bool, continue_activity, (), (override));
        MOCK_METHOD(bool, reset_activity, (), (override));
};
#pragma GCC diagnostic pop