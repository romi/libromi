#include "gmock/gmock.h"
#include "EventTimer.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockEventTimer : public romi::EventTimer
{
public:
        MOCK_METHOD(int, get_next_event, (), (override));
        MOCK_METHOD(void, set_timeout, (double timeout), (override));
        MOCK_METHOD(void, reset, (), (override));
};
#pragma GCC diagnostic pop