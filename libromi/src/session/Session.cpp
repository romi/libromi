#include <cstdlib>

#include "Linux.h"
#include "FileUtils.h"
#include "ClockAccessor.h"
#include "StringUtils.h"
#include "session/Session.h"
#include "data_provider/RoverIdentityProvider.h"


namespace romi {

        Session::Session(const rpp::ILinux &linux, const std::string &base_directory,
                     IRomiDeviceData &device_data, ISoftwareVersion &softwareVersion,
                     std::shared_ptr<ILocationProvider> location)
            :
            linux_(linux), base_directory_(), session_directory_(), device_data_(device_data), meta_folder_(),
            observation_id_(), roverIdentity_(std::make_unique<RoverIdentityProvider>(device_data, softwareVersion)), location_(location){

            auto currentdir = std::filesystem::current_path();
            base_directory_ = currentdir;
            base_directory_ /= base_directory;
            if (!std::filesystem::exists(base_directory_)) {
                    std::filesystem::create_directories(base_directory_);
            }
        }

        void Session::start(const std::string &observation_id) {
            observation_id_ = observation_id;
            // Create folder named "DeviceTypeXX_IDXX_DataTimeXX"
            std::string separator("_");
            std::string datatime = rpp::ClockAccessor::GetInstance()->datetime_compact_string();
            std::string session_dir = device_data_.RomiDeviceType() + separator + device_data_.RomiDeviceHardwareId() +
                                      separator + datatime;
            session_directory_.clear();
            session_directory_ = base_directory_ / session_dir;
            meta_folder_ = std::make_shared<MetaFolder>(roverIdentity_, location_, session_directory_);
        }

        void Session::stop() {
            session_directory_.clear();
        }

        bool Session::store_jpg(const std::string &name, Image &image) {
            bool retval = false;
            try{
                    meta_folder_->try_store_jpg(name, image, observation_id_);
                    retval = true;
            }
            catch (std::exception& ex) {
                throw;
            }
            return retval;
        }

        bool Session::store_png(const std::string &name, Image &image) {
            bool retval = false;
            try{
                    meta_folder_->try_store_png(name, image, observation_id_);
                    retval = true;
            }
            catch (std::exception& ex) {
                    throw;
            }
            return retval;
        }

        bool Session::store_svg(const std::string &name, const std::string &body) {
            bool retval = false;
            try{
                    meta_folder_->try_store_svg(name, body, observation_id_);
                    retval = true;
            }
            catch (std::exception& ex) {
                    throw;
            }
            return retval;
        }

        bool Session::store_txt(const std::string &name, const std::string &body) {
            bool retval = false;
            try{
                    meta_folder_->try_store_txt(name, body, observation_id_);
                    retval = true;
            }
            catch (std::exception& ex) {
                    throw;
            }
            return retval;
        }

        bool Session::store_path(const std::string &filename, int32_t path_number, Path &weeder_path) {
            bool retval = false;
            std::string filename_path_number = filename;
            const std::string format("%s-%05d");
            if (path_number > 0){
                    StringUtils::string_printf(filename_path_number, format.c_str(), filename.c_str(), path_number );

            }

            try{
                    meta_folder_->try_store_path(filename_path_number, weeder_path, observation_id_);
                    retval = true;
            }
            catch (std::exception& ex) {
                    throw;
            }
            return retval;
        }

        std::filesystem::path Session::current_path() {
            return session_directory_;
        }

}
