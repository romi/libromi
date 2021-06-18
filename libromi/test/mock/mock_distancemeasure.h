#include "gmock/gmock.h"
#include "rover/IDistanceMeasure.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockDistanceMeasure : public romi::IDistanceMeasure
{
public:
        MOCK_METHOD(bool, set_distance_to_navigate, (double distance), (override));
        MOCK_METHOD(double, get_distance_to_navigate, (), (override));
        MOCK_METHOD(bool, update_distance_estimate, (), (override));
        MOCK_METHOD(double, get_distance_from_start, (), (override));
        MOCK_METHOD(double, get_distance_to_end, (), (override));
};
#pragma GCC diagnostic pop
