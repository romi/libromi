#ifndef ROMI_ROVER_BUILD_AND_TEST_POINT_T_H
#define ROMI_ROVER_BUILD_AND_TEST_POINT_T_H

namespace romi
{
    class Point
    {
    public:
        Point(int32_t x, int32_t y) : x(x), y(y) {};
        int32_t x;
        int32_t y;
    };
}

#endif //ROMI_ROVER_BUILD_AND_TEST_POINT_T_H
