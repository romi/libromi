#ifndef ROMI_ROVER_BUILD_AND_TEST_METAFOLDER_H
#define ROMI_ROVER_BUILD_AND_TEST_METAFOLDER_H

#include "cv/ImageIO.h"
#include "data_provider/IRomiDeviceData.h"
#include "data_provider/ISoftwareVersion.h"
#include "data_provider/IIdentityProvider.h"
#include "data_provider/ILocationProvider.h"
#include "session/IMetaFolder.h"

namespace romi {

    class MetaFolder : public IMetaFolder {

    public:
        MetaFolder(std::shared_ptr<IIdentityProvider> identityProvider,
                   std::shared_ptr<ILocationProvider> locationProvider,
                   std::filesystem::path folder_path);

        // TBD: Refactor this out to the constuctor.
        void try_store_jpg(const std::string &filename, romi::Image &image, const std::string &observationId) override;
        void try_store_png(const std::string &filename, romi::Image &image, const std::string &observationId) override;
        void try_store_svg(const std::string &filename, const std::string& body, const std::string &observationId) override;
        void try_store_txt(const std::string &filename, const std::string& text, const std::string &observationId) override;
        void try_store_path(const std::string &filename, romi::Path &weeder_path, const std::string &observationId) override;

        inline static const std::string meta_data_filename_ = "meta_data.json";

    private:
        void try_create();
        std::filesystem::path build_filename_with_extension(const std::string& filename, const std::string& extension);
        void add_file_metadata(const std::string &filename, const std::string &ovservationId);
        void CheckInput(Image& image) const;
        void CheckInput(const std::string& string_data) const;
        std::shared_ptr<IIdentityProvider> identityProvider_;
        std::shared_ptr<ILocationProvider> locationProvider_;
        std::filesystem::path folderPath_;
        std::unique_ptr<JsonCpp> meta_data_;
        std::recursive_mutex metadata_file_mutex_;
    };

}


#endif //ROMI_ROVER_BUILD_AND_TEST_METAFOLDER_H
