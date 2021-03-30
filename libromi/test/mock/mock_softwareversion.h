#include "gmock/gmock.h"
#include "data_provider/ISoftwareVersion.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockSoftwareVersion : public romi::ISoftwareVersion
{
public:
        MOCK_METHOD(std::string, SoftwareVersionCurrent, (), (override));
        MOCK_METHOD(std::string, SoftwareVersionAlternate, (), (override));
};
#pragma GCC diagnostic pop