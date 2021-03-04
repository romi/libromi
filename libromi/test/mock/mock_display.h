#include "gmock/gmock.h"
#include "api/Display.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockDisplay : public romi::Display
{
public:
        MOCK_METHOD(bool, show, (size_t line, const std::string& display_string), (override));
        MOCK_METHOD(bool, clear, (size_t line), (override));
        MOCK_METHOD(size_t , count_lines, (), (override));
};
#pragma GCC diagnostic pop