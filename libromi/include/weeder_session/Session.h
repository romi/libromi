#ifndef ROMI_ROVER_BUILD_AND_TEST_SESSION_H
#define ROMI_ROVER_BUILD_AND_TEST_SESSION_H

#include <filesystem>
#include "ILinux.h"
#include "data_provider/IRomiDeviceData.h"
#include "data_provider/ISoftwareVersion.h"
#include "data_provider/ILocationProvider.h"
#include "MetaFolder.h"
#include "ISession.h"

const int INVALID_FD = -1;

namespace romi {

    class Session : public ISession {
    public:
        Session() = delete;

        explicit Session(rpp::ILinux &linux, const std::string &base_directory,
                         IRomiDeviceData &device_data, ISoftwareVersion &softwareVersion,
                         std::unique_ptr<ILocationProvider> location);
        ~Session() override = default;
        void start(const std::string &observation_id) override;
        void stop() override;
        bool store_jpg(const char* name, Image &image)  override;
        bool store_png(const char* name, Image &image)  override;
        bool store_svg(const char* name, const char *body, size_t len) override;
        bool store_txt(const char* name, const char *body, size_t len)  override;
        bool store_path(const std::string &filename, int32_t path_number, Path &weeder_path) override;

        // DEBUG
        void open_dump() override;
        void dump(const char *name, int32_t rows, int32_t cols, float *values) override;
        void dump(const char *name, int32_t rows, int32_t cols, double *values) override;
        void dump_interleave(const char *name, int32_t size, float *a, float *b) override;
        void dump_interleave(const char *name, int32_t size, double *a, double *b) override;
        void close_dump() override;
        void dump_int(int32_t value);
        void dump_text(const char *s, size_t len);
        void dump_double(double value);

    private:
        std::filesystem::path base_directory_;
        std::filesystem::path session_directory_;
        IRomiDeviceData &device_data_;
        std::shared_ptr<IMetaFolder> meta_folder_;
        std::string observation_id_;

        int _dump_fd;

    };

}

#endif //ROMI_ROVER_BUILD_AND_TEST_SESSION_H
