#ifndef ROMI_ROVER_BUILD_AND_TEST_IMETAFOLDER_H
#define ROMI_ROVER_BUILD_AND_TEST_IMETAFOLDER_H

#include <filesystem>
#include "cv/Image.h"
#include "api/Path.h"

namespace romi {

    class IMetaFolder {
    public:
        IMetaFolder() = default;
        virtual ~IMetaFolder() = default;
        virtual void try_create(const std::filesystem::path &path) = 0;
        virtual void try_store_jpg(const std::string &filename, romi::Image &image, const std::string &observationId) = 0;
        virtual void try_store_png(const std::string &filename, romi::Image &image, const std::string &observationId) = 0;
        virtual void
        try_store_svg(const std::string &filename, const std::string& body, const std::string &observationId) = 0;
        virtual void
        try_store_txt(const std::string &filename, const std::string& text, const std::string &observationId) = 0;
        virtual void
        try_store_path(const std::string &filename, romi::Path &weeder_path, const std::string &observationId) = 0;
    };

}

#endif //ROMI_ROVER_BUILD_AND_TEST_IMETAFOLDER_H
