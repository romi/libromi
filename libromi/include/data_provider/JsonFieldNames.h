#ifndef ROMI_ROVER_BUILD_AND_TEST_JSONFIELDNAMES_H
#define ROMI_ROVER_BUILD_AND_TEST_JSONFIELDNAMES_H

#include <string>
namespace JsonFieldNames
{
    // Location
    const std::string location = "location";
    const std::string latitude = "latitude";
    const std::string longitude = "longitude";
    const std::string x_position = "x";
    const std::string y_position = "y";

    // Identity
    const std::string romi_identity = "romi_identity";
    const std::string romi_device_type = "romi_device_type";
    const std::string romi_hardware_id = "romi_hardware_id";
    const std::string software_version_current = "software_version_current";
    const std::string software_version_alternate = "software_version_alternate";

    // MetaData
    const std::string filename = "filename";
    const std::string date_time = "date_time";
    const std::string observation_id = "observation_id";
}

#endif //ROMI_ROVER_BUILD_AND_TEST_JSONFIELDNAMES_H
