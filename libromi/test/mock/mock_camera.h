#include "gmock/gmock.h"
#include "api/ICamera.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockCamera : public romi::ICamera
{
public:
        MOCK_METHOD(bool, grab, (romi::Image &image), (override));
        MOCK_METHOD(rpp::MemBuffer&, grab_jpeg, (), (override));
};
#pragma GCC diagnostic pop
