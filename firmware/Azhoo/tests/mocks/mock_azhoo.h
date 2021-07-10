#include "gmock/gmock.h"
#include "IAzhoo.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockAzhoo : public IAzhoo
{
public:
        MOCK_METHOD(void, setup, (), (override));
        MOCK_METHOD(bool, configure, (AzhooConfiguration& config), (override));
        MOCK_METHOD(bool, enable, (), (override));
        MOCK_METHOD(bool, disable, (), (override));
        MOCK_METHOD(void, stop, (), (override));
        MOCK_METHOD(bool, set_target_speeds, (int16_t left, int16_t right), (override));
        MOCK_METHOD(bool, update, (), (override));
        MOCK_METHOD(void, get_encoders, (int32_t& left, int32_t& right, uint32_t& time), (override));
};
#pragma GCC diagnostic pop
