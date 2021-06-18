#include "gmock/gmock.h"
#include "rover/ITrackFollower.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockTrackFollower : public romi::ITrackFollower
{
public:
        MOCK_METHOD(bool, update_error_estimate, (), (override));
        MOCK_METHOD(double, get_cross_track_error, (), (override));
        MOCK_METHOD(double, get_orientation_error, (), (override));
};
#pragma GCC diagnostic pop
