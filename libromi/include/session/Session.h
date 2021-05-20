#ifndef ROMI_ROVER_BUILD_AND_TEST_SESSION_H
#define ROMI_ROVER_BUILD_AND_TEST_SESSION_H

#include <filesystem>
#include "ILinux.h"
#include "data_provider/IRomiDeviceData.h"
#include "data_provider/ISoftwareVersion.h"
#include "data_provider/ILocationProvider.h"
#include "session/MetaFolder.h"
#include "ISession.h"

namespace romi {

    class Session : public ISession {
    public:
        Session() = delete;

        explicit Session(const rpp::ILinux &linux, const std::string &base_directory,
                         IRomiDeviceData &device_data, ISoftwareVersion &softwareVersion,
                         std::unique_ptr<ILocationProvider> location);
        ~Session() override = default;
        void start(const std::string &observation_id) override;
        void stop() override;
        bool store_jpg(const std::string &name, Image &image)  override;
        bool store_png(const std::string &name, Image &image)  override;
        bool store_svg(const std::string &name, const std::string &body) override;
        bool store_txt(const std::string &name, const std::string &body) override;
        bool store_path(const std::string &filename, int32_t path_number, Path &weeder_path) override;
        std::filesystem::path current_path() override;

    private:
        const rpp::ILinux& linux_;
        std::filesystem::path base_directory_;
        std::filesystem::path session_directory_;
        IRomiDeviceData &device_data_;
        std::shared_ptr<IMetaFolder> meta_folder_;
        std::string observation_id_;
        std::unique_ptr<IIdentityProvider> roverIdentity_;

    };

}

#endif //ROMI_ROVER_BUILD_AND_TEST_SESSION_H
