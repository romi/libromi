#ifndef ROMI_ROVER_BUILD_AND_TEST_ISOFTWAREVERSION_H
#define ROMI_ROVER_BUILD_AND_TEST_ISOFTWAREVERSION_H

#include <string>

class ISoftwareVersion {
        public:
                virtual ~ISoftwareVersion() = default;
                virtual std::string SoftwareVersionCurrent() = 0;
                virtual std::string SoftwareVersionAlternate() = 0;
};


#endif //ROMI_ROVER_BUILD_AND_TEST_SOFTWAREVERSION_H
