#include <cstdlib>

#include "Linux.h"
#include "FileUtils.h"
#include "ClockAccessor.h"
#include "StringUtils.h"
#include "weeder/Session.h"
#include "data_provider/RoverIdentityProvider.h"


namespace romi {

        Session::Session(rpp::ILinux &linux, const std::string &base_directory,
                     IRomiDeviceData &device_data, ISoftwareVersion &softwareVersion,
                     std::unique_ptr<ILocationProvider> location)
            :
            base_directory_(), session_directory_(), device_data_(device_data), meta_folder_(), observation_id_(), _dump_fd(INVALID_FD) {

            auto roverIdentity = std::make_unique<RoverIdentityProvider>(device_data, softwareVersion);
            // TBD: Keep these variables local. Then make a new metafolder on start. Avoid use without create.
            meta_folder_ = std::make_unique<MetaFolder>(std::move(roverIdentity), std::move(location));

            auto homedir = FileUtils::TryGetHomeDirectory(linux);
            base_directory_ = homedir;
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
            session_directory_ = base_directory_ /= session_dir;
            meta_folder_->try_create(session_directory_);
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


        // ToDo: DEBUG Functions. Refactor out

        void Session::dump_int(int32_t value)
        {
            ssize_t n = write(_dump_fd, (void *) &value, 4);
            if (n != 4)
                    r_err("dump: write error");
        }

        void Session::dump_text(const char *s, size_t len)
        {
            ssize_t n = write(_dump_fd, s, len);
            if (n != static_cast<ssize_t>(len))
                    r_err("dump: write error");
        }

        void Session::dump_double(double value)
        {
            ssize_t n = write(_dump_fd, (void *) &value, sizeof(double));
            if (n != (ssize_t) sizeof(double))
                    r_err("dump: write error");
        }

        void Session::open_dump()
        {
            close_dump();
            _dump_fd = ::open("dump.out",
                              O_WRONLY | O_CREAT | O_TRUNC,
                              S_IRUSR | S_IWUSR);
            if (_dump_fd == -1)
                    r_err("DebugWeedingFolder::open_dump: failed to open dump.out");
        }

        void Session::dump(const char *name, int32_t rows,
                                       int32_t cols, float *values)
        {
                if (_dump_fd != INVALID_FD) {
                        size_t len = strlen(name);
                        if (len > 31)
                                len = 31;
                        dump_int(static_cast<int>(len));
                        dump_text(name, len);
                        dump_int(rows);
                        dump_int(cols);

                        int index = 0;
                        for (int row = 0; row < rows; row++) {
                                for (int col = 0; col < cols; col++) {
                                        dump_double(values[index]);
                                        index++;
                                }
                        }
                }
        }

        void Session::dump(const char *name, int32_t rows,
                                  int32_t cols, double *values)
        {
            if (_dump_fd != INVALID_FD) {
                    size_t len = strlen(name);
                    if (len > 31)
                            len = 31;
                    dump_int(static_cast<int32_t>(len));
                    dump_text(name, len);
                    dump_int(rows);
                    dump_int(cols);

                    int index = 0;
                    for (int row = 0; row < rows; row++) {
                            for (int col = 0; col < cols; col++) {
                                    dump_double(values[index]);
                                    index++;
                            }
                    }
            }
        }

        void Session::dump_interleave(const char *name, int32_t size,
                                                  float *a, float *b)
        {
                if (_dump_fd != -1) {
                        size_t len = strlen(name);
                        if (len > 31)
                                len = 31;
                        dump_int(static_cast<int32_t>(len));
                        dump_text(name, len);
                        dump_int(size);
                        dump_int(2);

                        for (int i = 0; i < size; i++) {
                                dump_double(a[i]);
                                dump_double(b[i]);
                        }
                }
        }

        void Session::dump_interleave(const char *name, int32_t size,
                                             double *a, double *b)
        {
            if (_dump_fd != -1) {
                    size_t len = strlen(name);
                    if (len > 31)
                            len = 31;
                    dump_int(static_cast<int32_t>(len));
                    dump_text(name, len);
                    dump_int(size);
                    dump_int(2);

                    for (int i = 0; i < size; i++) {
                            dump_double(a[i]);
                            dump_double(b[i]);
                    }
            }
        }

        void Session::close_dump()
        {
            if (_dump_fd != -1) {
                    ::close(_dump_fd);
                    _dump_fd = -1;
            }
        }

}
