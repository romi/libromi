#ifndef ROMI_ROVER_BUILD_AND_TEST_JSONFIELDNAMES_H
#define ROMI_ROVER_BUILD_AND_TEST_JSONFIELDNAMES_H

#include <string_view>
namespace JsonFieldNames
{
    // Location
    const std::string_view location = "location";
    const std::string_view latitude = "latitude";
    const std::string_view longitude = "longitude";

    // Identity
    const std::string_view identity = "identity";
    const std::string_view romi_device_type = "romi_device_type";
    const std::string_view romi_hardware_id = "romi_hardware_id";
//    const std::string_view software_version = "software_version";
    const std::string_view software_version_current = "software_version_current";
    const std::string_view software_version_alternate = "software_version_alternate";
}

#endif //ROMI_ROVER_BUILD_AND_TEST_JSONFIELDNAMES_H
