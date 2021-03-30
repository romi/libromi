#ifndef ROMI_ROVER_BUILD_AND_TEST_METAFOLDER_H
#define ROMI_ROVER_BUILD_AND_TEST_METAFOLDER_H

#include "IMetaFolder.h"

#include "data_provider/IRomiDeviceData.h"
#include "data_provider/ISoftwareVersion.h"
#include "data_provider/IIdentityProvider.h"
#include "data_provider/ILocationProvider.h"

namespace romi {

    class MetaFolder : public IMetaFolder {

    public:
        MetaFolder(std::unique_ptr<IIdentityProvider> identityProvider,
                   std::unique_ptr<ILocationProvider> locationProvider);
        void try_create(const std::filesystem::path &path) override;
        void try_store_jpg(const std::string &filename, romi::Image &image) override;
        void try_store_png(const std::string &filename, romi::Image &image) override;
        void try_store_svg(const std::string &filename, const char *body, size_t len) override;
        void try_store_txt(const std::string &filename, const char *body, size_t len) override;
        void try_store_path(const std::string &filename, romi::Path &weeder_path) override;

    private:
        std::unique_ptr<IIdentityProvider> identityProvider_;
        std::unique_ptr<ILocationProvider> locationProvider_;
        std::filesystem::path path_;
        std::unique_ptr<JsonCpp> meta_data_;
        inline static const std::string meta_data_filename_ = "meta_data.txt";
    };

}


#endif //ROMI_ROVER_BUILD_AND_TEST_METAFOLDER_H
