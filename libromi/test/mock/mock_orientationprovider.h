#include "gmock/gmock.h"
#include "data_provider/IOrientationProvider.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

class MockOrientationProvider : public romi::IOrientationProvider
{
public:
        MOCK_METHOD(bool, update_orientation_estimate, (), (override));
        MOCK_METHOD(double, get_orientation, (), (override));

};
#pragma GCC diagnostic pop
