#include "data_provider/RomiDeviceData.h"

namespace romi {

    std::string RomiDeviceData::RomiDeviceType() {
            return std::string("Rover");
    }

    std::string RomiDeviceData::RomiDeviceHardwareId() {
            return std::string("deadbeef");
    }

}