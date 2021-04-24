#ifndef ROMI_ROVER_BUILD_AND_TEST_CONFIGURATIONPROVIDER_H
#define ROMI_ROVER_BUILD_AND_TEST_CONFIGURATIONPROVIDER_H

#include "JsonCpp.h"
#include <rover/RoverOptions.h>

namespace romi
{
    std::string get_brush_motor_device_in_config(JsonCpp& config);
    std::string get_brush_motor_device(romi::IOptions& options, JsonCpp& config);
    std::string get_sound_font_in_config(JsonCpp& config);
    std::string get_sound_font_file(romi::IOptions& options, JsonCpp& config);
    std::string get_script_file_in_config(JsonCpp& config);
    std::string get_script_file(romi::IOptions& options, JsonCpp& config);
    std::string get_session_directory(romi::IOptions& options, JsonCpp& config);
    std::string get_camera_image(romi::IOptions& options, JsonCpp& config);
    std::string get_camera_device_in_config(JsonCpp& config);
    std::string get_camera_device(romi::IOptions& options, JsonCpp& config);

}

#endif //ROMI_ROVER_BUILD_AND_TEST_CONFIGURATIONPROVIDER_H
