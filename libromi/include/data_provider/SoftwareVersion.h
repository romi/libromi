#ifndef ROMI_ROVER_BUILD_AND_TEST_SOFTWAREVERSION_H
#define ROMI_ROVER_BUILD_AND_TEST_SOFTWAREVERSION_H

#include <string>
#include "ISoftwareVersion.h"

namespace romi {

    class SoftwareVersion : public ISoftwareVersion {
    public:
        ~SoftwareVersion() override = default;
        std::string SoftwareVersionCurrent() override;
        std::string SoftwareVersionAlternate() override;
    };

}

#endif //ROMI_ROVER_BUILD_AND_TEST_SOFTWAREVERSION_H
