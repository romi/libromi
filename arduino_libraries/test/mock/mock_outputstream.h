#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "IOutputStream.h"

#ifndef __MOCK_OUTPUTSTREAM_H
#define __MOCK_OUTPUTSTREAM_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockOutputStream : public romiserial::IOutputStream
{
public:
        MOCK_METHOD1(write, bool(char c));
};
#pragma GCC diagnostic pop

#endif // __MOCK_OUTPUTSTREAM_H
