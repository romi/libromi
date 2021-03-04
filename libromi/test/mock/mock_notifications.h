#include "gmock/gmock.h"
#include "api/Notifications.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockNotifications : public romi::Notifications
{
public:
        MOCK_METHOD(void, notify, (const char *name), (override));
        MOCK_METHOD(void, stop, (const char *name), (override));
        MOCK_METHOD(void, reset, (), (override));
};
#pragma GCC diagnostic pop