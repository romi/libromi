
#include <iostream>
#include "weeder_session/MetaFolder.h"


namespace romi {
    MetaFolder::MetaFolder(std::unique_ptr<IIdentityProvider> identityProvider,
                           std::unique_ptr<ILocationProvider> locationProvider)
        : identityProvider_(std::move(identityProvider)), locationProvider_(std::move(locationProvider)),
        path_(), meta_data_() {

        if(nullptr == identityProvider_){
                throw std::invalid_argument("identityProvider");
        }
        if(nullptr == locationProvider_){
                throw std::invalid_argument("locationProvider");
        }

    }

    void MetaFolder::try_create(const std::filesystem::path& path) {
            path_ = path;
            std::filesystem::create_directories(path_);
            meta_data_ = std::make_unique<JsonCpp>();
            std::string identity = identityProvider_->identity();
            auto meta_data_object = json_string_create(identity.c_str());
            json_tofile(meta_data_object, k_json_pretty, (path_ / meta_data_filename_).c_str());
            json_unref(meta_data_object);
    }

    void MetaFolder::try_store_jpg(const std::string &filename, romi::Image &image) {
            std::cout << filename << image.length() << std::endl;
    }

    void MetaFolder::try_store_png(const std::string &filename, romi::Image &image) {
            std::cout << filename << image.length() << std::endl;
    }

    void MetaFolder::try_store_svg(const std::string &filename, const char *body, size_t len) {
            std::cout << filename << (body == nullptr) << len << std::endl;
    }

    void MetaFolder::try_store_txt(const std::string &filename, const char *body, size_t len) {
            std::cout << filename << (body == nullptr) << len << std::endl;
    }

    void MetaFolder::try_store_path(const std::string &filename, romi::Path &weeder_path) {
            std::cout << filename << weeder_path.size() << std::endl;
    }

}
