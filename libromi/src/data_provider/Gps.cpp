#include "data_provider/Gps.h"

// Sony CSL Paris Coordinates.
const double fixed_latitude = 48.843689;
const double fixed_longitude = 2.3476473;

namespace romi {

    void Gps::CurrentLocation(double &latitude, double &longitude) {

            latitude = fixed_latitude;
            longitude = fixed_longitude;
    }

}