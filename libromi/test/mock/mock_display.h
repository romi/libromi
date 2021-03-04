#include "gmock/gmock.h"
#include "api/Display.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockDisplay : public romi::Display
{
public:
        MOCK_METHOD(bool, show, (int line, const char *s), (override));
        MOCK_METHOD(bool, clear, (int line), (override));
        MOCK_METHOD(int, count_lines, (), (override));
};
#pragma GCC diagnostic pop