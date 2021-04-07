
#ifndef ROMI_ROVER_BUILD_AND_TEST_GPS_H
#define ROMI_ROVER_BUILD_AND_TEST_GPS_H

#include "data_provider/IGPS.h"

namespace romi {

    class Gps : public IGps{
    public:
        Gps() = default;
        virtual ~Gps() = default;
        void CurrentLocation(double &latitude, double &longitude) override;

    };

}
#endif //ROMI_ROVER_BUILD_AND_TEST_GPS_H
