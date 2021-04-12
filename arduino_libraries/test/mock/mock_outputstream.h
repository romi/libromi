#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "IOutputStream.h"

#ifndef __MOCK_OUTPUTSTREAM_H
#define __MOCK_OUTPUTSTREAM_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockOutputStream : public IOutputStream
{
public:
        MOCK_METHOD1(write, bool(char c));
        MOCK_METHOD2(write, bool(const uint8_t *data, size_t length));
        MOCK_METHOD1(print, size_t(const char *s));
};
#pragma GCC diagnostic pop

#endif // __MOCK_OUTPUTSTREAM_H
