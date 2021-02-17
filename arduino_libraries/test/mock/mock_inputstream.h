#include "gmock/gmock.h"
#include "IInputStream.h"

#ifndef __MOCK_INPUTSTREAM_H
#define __MOCK_INPUTSTREAM_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockInputStream : public IInputStream
{
public:
        MOCK_METHOD(int, available, (), (override));
        MOCK_METHOD(int, readchar, (char& c), (override));
        MOCK_METHOD(bool, readline, (char *buffer, size_t buflen), (override));
        MOCK_METHOD(void, set_timeout, (float seconds), (override));
};
#pragma GCC diagnostic pop
#endif // __MOCK_INPUTSTREAM_H
