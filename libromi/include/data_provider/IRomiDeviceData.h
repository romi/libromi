#ifndef ROMI_ROVER_BUILD_AND_TEST_IROMIDEVICEDATA_H
#define ROMI_ROVER_BUILD_AND_TEST_IROMIDEVICEDATA_H
#include <string>

class IRomiDeviceData {
        public:
                virtual ~IRomiDeviceData() = default;
                virtual std::string RomiDeviceType() = 0;
                virtual std::string RomiDeviceHardwareId() = 0;
};


#endif //ROMI_ROVER_BUILD_AND_TEST_ROMIDEVICEDATA_H
