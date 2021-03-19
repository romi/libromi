#ifndef ROMI_ROVER_BUILD_AND_TEST_ROVERIDENTITYPROVIDER_H
#define ROMI_ROVER_BUILD_AND_TEST_ROVERIDENTITYPROVIDER_H

#include "IIdentityProvider.h"
#include "ISoftwareVersion.h"
#include "IRomiDeviceData.h"

class RoverIdentityProvider : public IIdentityProvider{
        public:
                explicit RoverIdentityProvider(IRomiDeviceData& deviceData, ISoftwareVersion& softwareVersion);
                ~RoverIdentityProvider() override = default;
                std::string identity() override;
        private:
                std::string romi_device_type_;
                std::string romi_hardware_id_;
                std::string current_software_version_;
                std::string alternate_software_version_;

                IRomiDeviceData& device_data_;
                ISoftwareVersion& software_version_;
};


#endif //ROMI_ROVER_BUILD_AND_TEST_ROVERIDENTITYPROVIDER_H
