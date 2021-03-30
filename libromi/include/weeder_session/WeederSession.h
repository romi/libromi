#ifndef ROMI_ROVER_BUILD_AND_TEST_WEEDERSESSION_H
#define ROMI_ROVER_BUILD_AND_TEST_WEEDERSESSION_H

#include <filesystem>
#include "ILinux.h"
#include "data_provider/IRomiDeviceData.h"
#include "data_provider/ISoftwareVersion.h"
#include "data_provider/ILocationProvider.h"
#include "MetaFolder.h"
#include "IWeederSession.h"

namespace romi {

    class WeederSession : public IWeederSession {
    public:
        WeederSession() = delete;

        explicit WeederSession(rpp::ILinux &linux, const std::string &base_directory,
                               IRomiDeviceData &device_data, ISoftwareVersion &softwareVersion,
                               std::unique_ptr<ILocationProvider> location);
        ~WeederSession() override = default;
        void Start(const std::string &observation_id) override;
        void Stop() override;

    private:
        std::filesystem::path base_directory_;
        std::filesystem::path session_directory_;
        IRomiDeviceData &device_data_;
        std::shared_ptr<IMetaFolder> meta_folder_;
        std::string observation_id_;
    };

}

#endif //ROMI_ROVER_BUILD_AND_TEST_WEEDERSESSION_H
