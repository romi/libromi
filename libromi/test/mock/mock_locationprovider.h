#include "gmock/gmock.h"
#include "data_provider/ILocationProvider.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

class MockLocationProvider : public romi::ILocationProvider
{
public:
        MOCK_METHOD(std::string, get_location_string, (), (override));
        MOCK_METHOD(bool, update_location_estimate, (), (override));
        MOCK_METHOD(romi::v3, get_location, (), (override));

};
#pragma GCC diagnostic pop
