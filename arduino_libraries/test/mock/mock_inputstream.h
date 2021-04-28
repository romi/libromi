#include "gmock/gmock.h"
#include "IInputStream.h"

#ifndef __MOCK_INPUTSTREAM_H
#define __MOCK_INPUTSTREAM_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockInputStream : public romiserial::IInputStream
{
public:
        MOCK_METHOD(bool, available, (), (override));
        MOCK_METHOD(bool, read, (char& c), (override));
        MOCK_METHOD(void, set_timeout, (double seconds), (override));
};
#pragma GCC diagnostic pop
#endif // __MOCK_INPUTSTREAM_H
