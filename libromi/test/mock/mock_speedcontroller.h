#include "gmock/gmock.h"
#include "SpeedController.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockSpeedController : public romi::SpeedController
{
public:
        MOCK_METHOD(bool, stop, (), (override));
        MOCK_METHOD(bool, drive_at, (double speed, double direction), (override));
        MOCK_METHOD(bool, drive_accurately_at, (double speed, double direction), (override));
        MOCK_METHOD(bool, spin, (double direction), (override));
};
#pragma GCC diagnostic pop