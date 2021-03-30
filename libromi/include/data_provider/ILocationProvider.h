#ifndef ROMI_ROVER_BUILD_AND_TEST_ILOCATIONPROVIDER_H
#define ROMI_ROVER_BUILD_AND_TEST_ILOCATIONPROVIDER_H

#include <string>

namespace romi {

    class ILocationProvider {
    public:
        ILocationProvider() = default;
        virtual ~ILocationProvider() = default;
        virtual std::string location() = 0;
    };

}
#endif //ROMI_ROVER_BUILD_AND_TEST_ILOCATIONPROVIDER_H
