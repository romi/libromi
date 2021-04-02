#include "data_provider/JsonFieldNames.h"
#include "data_provider/RoverIdentityProvider.h"
#include "JsonCpp.h"

namespace romi {

    RoverIdentityProvider::RoverIdentityProvider(IRomiDeviceData &deviceData, ISoftwareVersion &softwareVersion) :
                    romi_device_type_(), romi_hardware_id_(),
                    current_software_version_(), alternate_software_version_(),
                    device_data_(deviceData), software_version_(softwareVersion) {

            current_software_version_ = software_version_.SoftwareVersionCurrent();
            alternate_software_version_ = software_version_.SoftwareVersionAlternate();
            romi_device_type_ = device_data_.RomiDeviceType();
            romi_hardware_id_ = device_data_.RomiDeviceHardwareId();
    }

    std::string RoverIdentityProvider::identity() {

            json_object_t identity_data_object = json_object_create();
            json_object_setstr(identity_data_object, JsonFieldNames::romi_device_type.data(),
                               romi_device_type_.c_str());
            json_object_setstr(identity_data_object, JsonFieldNames::romi_hardware_id.data(),
                               romi_hardware_id_.c_str());
            json_object_setstr(identity_data_object, JsonFieldNames::software_version_current.data(),
                               current_software_version_.c_str());
            json_object_setstr(identity_data_object, JsonFieldNames::software_version_alternate.data(),
                               alternate_software_version_.c_str());

            std::string identityString;
            JsonCpp identity(identity_data_object);
            identity.tostring(identityString, k_json_pretty);

            json_unref(identity_data_object);
            return identityString;
    }

}