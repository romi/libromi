#include "gmock/gmock.h"
#include "api/IDisplay.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockDisplay : public romi::IDisplay
{
public:
        MOCK_METHOD(bool, show, (size_t line, const std::string& display_string), (override));
        MOCK_METHOD(bool, clear, (size_t line), (override));
        MOCK_METHOD(size_t , count_lines, (), (override));
};
#pragma GCC diagnostic pop