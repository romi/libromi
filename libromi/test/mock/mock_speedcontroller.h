#include "gmock/gmock.h"
#include "ISpeedController.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockSpeedController : public romi::ISpeedController
{
public:
        MOCK_METHOD(bool, stop, (), (override));
        MOCK_METHOD(bool, drive_at, (double speed, double direction), (override));
        MOCK_METHOD(bool, drive_accurately_at, (double speed, double direction), (override));
        MOCK_METHOD(bool, spin, (double direction), (override));
};
#pragma GCC diagnostic pop