#include <cstdlib>
#include <iostream>

#include "Linux.h"
#include "FileUtils.h"
#include "ClockAccessor.h"
#include "weeder_session/WeederSession.h"
#include "data_provider/RoverIdentityProvider.h"

namespace romi {
    WeederSession::WeederSession(rpp::ILinux &linux, const std::string &base_directory,
                                 IRomiDeviceData &device_data, ISoftwareVersion &softwareVersion,
                                 std::unique_ptr<ILocationProvider> location)
            :
            base_directory_(), session_directory_(), device_data_(device_data), meta_folder_(),
            observation_id_() {

            auto roverIdentity = std::make_unique<RoverIdentityProvider>(device_data, softwareVersion);
            meta_folder_ = std::make_unique<MetaFolder>(std::move(roverIdentity), std::move(location));

            auto homedir = FileUtils::TryGetHomeDirectory(linux);
            base_directory_ = homedir;
            base_directory_ /= base_directory;
            if (!std::filesystem::exists(base_directory_)) {
                    std::filesystem::create_directories(base_directory_);
            }
    }

    void WeederSession::Start(const std::string &observation_id) {
            observation_id_ = observation_id;
            // Create folder named "DeviceTypeXX_IDXX_DataTimeXX"
            std::string separator("_");
            std::string datatime = rpp::ClockAccessor::GetInstance()->datetime_compact_string();
            std::string session_dir = device_data_.RomiDeviceType() + separator + device_data_.RomiDeviceHardwareId() +
                                      separator + datatime;
            session_directory_.clear();
            session_directory_ = base_directory_ /= session_dir;
            meta_folder_->try_create(session_directory_);
    }

    void WeederSession::Stop() {
            session_directory_.clear();
    }

}
