#include "gmock/gmock.h"
#include "IScriptEngine.h"
#include "rover/Rover.h"

class MockScriptEngine : public romi::IScriptEngine<romi::Rover>
{
        MOCK_METHOD(int, get_next_event, (), (override));
        MOCK_METHOD(void, execute_script, (romi::Rover& target, size_t index), (override));
};
