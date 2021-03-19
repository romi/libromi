#ifndef ROMI_ROVER_BUILD_AND_TEST_IGPS_H
#define ROMI_ROVER_BUILD_AND_TEST_IGPS_H

class IGps
{
        public:
                virtual ~IGps() = default;
                virtual void CurrentLocation(double& latitude, double& longitude) = 0;
};
#endif //ROMI_ROVER_BUILD_AND_TEST_IGPS_H
