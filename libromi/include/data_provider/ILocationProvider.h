#ifndef ROMI_ROVER_BUILD_AND_TEST_ILOCATIONPROVIDER_H
#define ROMI_ROVER_BUILD_AND_TEST_ILOCATIONPROVIDER_H

#include <string>
#include "v3.h"

namespace romi {

    class ILocationProvider {
    public:
        ILocationProvider() = default;
        virtual ~ILocationProvider() = default;
        
        virtual std::string get_location_string() = 0;
        
        virtual bool update_location_estimate() = 0;
        virtual v3 get_location() = 0;
    };

}
#endif // ROMI_ROVER_BUILD_AND_TEST_ILOCATIONPROVIDER_H
