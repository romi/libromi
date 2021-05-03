#ifndef ROMI_ROVER_BUILD_AND_TEST_ISESSION_H
#define ROMI_ROVER_BUILD_AND_TEST_ISESSION_H

#include <filesystem>
#include <string>
#include "cv/Image.h"
#include "api/Path.h"

namespace romi {

    class ISession {
    public:
        ISession() = default;
        virtual ~ISession() = default;
        virtual void start(const std::string &observation_id) = 0;
        virtual void stop() = 0;
        virtual bool store_jpg(const std::string &name, Image &image)  = 0;
        virtual bool store_png(const std::string &name, Image &image)  = 0;
        virtual bool store_svg(const std::string &name, const std::string &body) = 0;
        virtual bool store_txt(const std::string &name, const std::string &body) = 0;
        virtual bool store_path(const std::string &filename, int32_t path_number, Path &weeder_path) = 0;
        virtual std::filesystem::path current_path() = 0;

    };

}

#endif //ROMI_ROVER_BUILD_AND_TEST_SESSION_H
