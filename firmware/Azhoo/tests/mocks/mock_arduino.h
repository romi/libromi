#include "gmock/gmock.h"
#include "IArduino.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockArduino : public IArduino
{
public:
        MOCK_METHOD(IEncoder&, left_encoder, (), (override));
        MOCK_METHOD(IEncoder&, right_encoder, (), (override));
        MOCK_METHOD(IPWM&, left_pwm, (), (override));
        MOCK_METHOD(IPWM&, right_pwm, (), (override));
        MOCK_METHOD(uint32_t, milliseconds, (), (override));
};
#pragma GCC diagnostic pop
