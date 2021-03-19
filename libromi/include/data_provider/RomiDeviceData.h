#ifndef ROMI_ROVER_BUILD_AND_TEST_ROMIDEVICEDATA_H
#define ROMI_ROVER_BUILD_AND_TEST_ROMIDEVICEDATA_H

#include "IRomiDeviceData.h"

class RomiDeviceData : public IRomiDeviceData{
    std::string RomiDeviceType() override;
    std::string RomiDeviceHardwareId() override;
};


#endif //ROMI_ROVER_BUILD_AND_TEST_ROMIDEVICEDATA_H
