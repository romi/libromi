/*
  romi-rover

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  romi-rover is collection of applications for the Romi Rover.

  romi-rover is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */

#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "util/FileUtils.h"
#include "util/Logger.h"
#include "camera/LibCamera.h"

namespace romi {

        using SynchonizedCodeBlock = std::lock_guard<std::mutex>;

        LibCamera::LibCamera()
                : manager_(),
                  camera_(),
                  allocator_(nullptr),
                  stream_(nullptr),
                  request_(),
                  pixel_format_(libcamera::formats::RGB888),
                  mutex_(),
                  cv_(),
                  request_completed_(false),
                  image_(),
                  jpeg_()
        {
                manager_ = std::make_unique<libcamera::CameraManager>();
                manager_->start();

                auto cameras = manager_->cameras();
                if (cameras.empty()) {
                        manager_->stop();
                        throw std::runtime_error("LibCamera: No cameras were "
                                                 "identified on the system.");
                }

                std::string cameraId = cameras[0]->id();
                camera_ = manager_->get(cameraId);
                camera_->acquire();
                
                std::unique_ptr<libcamera::CameraConfiguration> config
                        = camera_->generateConfiguration( { libcamera::StreamRole::StillCapture } );

                libcamera::StreamConfiguration &streamConfig = config->at(0);
                std::cout << "Default stream configuration is: "
                          << streamConfig.toString() << std::endl;
        
                // streamConfig.size.width = 640;
                // streamConfig.size.height = 480;
                streamConfig.pixelFormat = pixel_format_;
        
                libcamera::CameraConfiguration::Status status = config->validate();
                if (status == libcamera::CameraConfiguration::Status::Invalid) {
                        manager_->stop();
                        throw std::runtime_error("LibCamera: Failed to validate "
                                                 "the stream configuration.");
                } else if (status == libcamera::CameraConfiguration::Status::Adjusted) {
                        std::cout << "The stream configuration was adjusted." << std::endl;
                        std::cout << "Validated stream configuration is: "
                                  << streamConfig.toString() << std::endl;

                        // FIXME: Check whether we can handle the selected pixel format
                        pixel_format_ = streamConfig.pixelFormat;
                        assert_format();
                        
                } else {
                        std::cout << "Validated stream configuration is: "
                                  << streamConfig.toString() << std::endl;
                }

                width_ = streamConfig.size.width;
                height_ = streamConfig.size.height;
                
                camera_->configure(config.get());
                
                stream_ = streamConfig.stream();
                allocator_ = new libcamera::FrameBufferAllocator(camera_);

                int ret = allocator_->allocate(stream_);
                if (ret < 0) {
                        manager_->stop();
                        std::runtime_error("LibCamera: Can't allocate buffers");
                }
                
                const std::vector<std::unique_ptr<libcamera::FrameBuffer>> &buffers = allocator_->buffers(stream_);
                
                request_ = camera_->createRequest();
                if (!request_) {
                        manager_->stop();
                        std::runtime_error("LibCamera: Can't create request");
                }

                const std::unique_ptr<libcamera::FrameBuffer> &buffer = buffers[0];
                ret = request_->addBuffer(stream_, buffer.get());
                if (ret < 0) {
                        manager_->stop();
                        std::runtime_error("LibCamera: Can't set buffer for request");
                }

                camera_->requestCompleted.connect(this, &LibCamera::request_complete);
                camera_->start();
        }

        LibCamera::~LibCamera()
        {
                camera_->stop();
                allocator_->free(stream_);
                delete allocator_;
                camera_->release();
                camera_.reset();
                manager_->stop();
        }

        void LibCamera::assert_format()
        {
                if (pixel_format_ == libcamera::formats::RGB888) {
                        r_info("LibCamera: RGB888 format");
                } else if (pixel_format_ == libcamera::formats::MJPEG) {
                        r_info("LibCamera: MJPEG format");                        
                } else {
                        throw std::runtime_error("LibCamera: Unsupported format");
                }
        }

        bool LibCamera::set_value(const std::string& name, double value)
        {
                (void) name;
                (void) value;
                return true;
        }
        
        bool LibCamera::select_option(const std::string& name,
                                       const std::string& value)
        {
                (void) name;
                (void) value;
                return true;
        }

        void LibCamera::send_request()
        {
                r_debug("LibCamera::send_request");
                request_completed_ = false;
                camera_->queueRequest(request_.get());
        }

        void LibCamera::wait_request_completed()
        {
                r_debug("LibCamera::wait_request_completed");
                // semaphore_.acquire();
                r_debug("LibCamera::wait_request_completed: OK");
        }

        void LibCamera::signal_request_completed()
        {
                r_debug("LibCamera::signal_request_completed");
                request_completed_ = true;
                cv_.notify_one();
        }

        void LibCamera::request_complete(libcamera::Request *request)
        {
                r_debug("LibCamera::request_complete");
                process_request_buffer(request);
                signal_request_completed();
        }

        void LibCamera::process_request_buffer(libcamera::Request *request)
        {
                r_debug("LibCamera::process_request_buffer");
        
                if (request->status() == libcamera::Request::RequestCancelled)
                        return;
                
                const std::map<const libcamera::Stream *, libcamera::FrameBuffer *> &buffers = request->buffers();
                
                for (auto bufferPair : buffers) {
                        libcamera::FrameBuffer *buffer = bufferPair.second;
                        //const FrameMetadata &metadata = buffer->metadata();
                        for (const libcamera::FrameBuffer::Plane &plane : buffer->planes()) {

                                int mmapFlags = PROT_READ;
                                size_t mapLength = 0;
                                size_t dmabufLength = 0;
                                const int fd = plane.fd.get();
                                dmabufLength = lseek(fd, 0, SEEK_END);
                                if (plane.offset > dmabufLength ||
                                    plane.offset + plane.length > dmabufLength) {
                                        std::cerr << "plane is out of buffer: buffer length="
                                                  << dmabufLength << ", plane offset=" << plane.offset
                                                  << ", plane length=" << plane.length
                                                  << std::endl;
                                        return;
                                }

                                mapLength = (size_t) (plane.offset + plane.length);
                                void *map_address = mmap(nullptr, mapLength, mmapFlags,
                                                         MAP_SHARED, fd, 0);
                                if (map_address == MAP_FAILED) {
                                        r_err("LibCamera: Failed to mmap plane: %s",
                                              strerror(errno));
                                        return;
                                }

                                const uint8_t *data = (const uint8_t *) map_address;
                                import_data(data + plane.offset, plane.length);

                                munmap(map_address, mapLength);
                        }

                        std::cout << std::endl;
                }
                
                request->reuse(libcamera::Request::ReuseBuffers);
        }

        void LibCamera::import_data(const uint8_t *data, size_t length)
        {
                if (pixel_format_ == libcamera::formats::RGB888) {
                        import_rgb(data, length);
                } else if (pixel_format_ == libcamera::formats::MJPEG) {
                        import_jpeg(data, length);
                }
        }

        void LibCamera::import_jpeg(const uint8_t *data, size_t length)
        {
                r_debug("LibCamera::import_jpeg");
                jpeg_.clear();
                jpeg_.append(data, length);
        }

        void LibCamera::import_rgb(const uint8_t *data, size_t)
        {
                r_debug("LibCamera::import_rgb");
                image_.import(Image::RGB, data, width_, height_);
        }
        
        bool LibCamera::grab(Image &image)
        {
                r_debug("LibCamera::grab");
                std::unique_lock<std::mutex> lk(mutex_);
                send_request();
                // wait_request_completed();
                cv_.wait(lk, [this]{ return request_completed_; });
                convert_jpeg_to_rgb_perhaps();
                image = image_;
                return true;
        }

        rcom::MemBuffer& LibCamera::grab_jpeg()
        {
                r_debug("LibCamera::grab_jpeg");
                std::unique_lock<std::mutex> lk(mutex_);
                send_request();
                //wait_request_completed();
                cv_.wait(lk, [this]{ return request_completed_; });
                convert_rgb_to_jpeg_perhaps();
                return jpeg_;
        }

        void LibCamera::convert_jpeg_to_rgb_perhaps()
        {
                if (pixel_format_ == libcamera::formats::RGB888) {
                        return;
                } else if (pixel_format_ == libcamera::formats::MJPEG) {
                        convert_jpeg_to_rgb();
                }
        }

        void LibCamera::convert_jpeg_to_rgb()
        {
        }

        void LibCamera::convert_rgb_to_jpeg_perhaps()
        {
                if (pixel_format_ == libcamera::formats::RGB888) {
                        convert_rgb_to_jpeg();
                } else if (pixel_format_ == libcamera::formats::MJPEG) {
                        return;
                }
        }

        void LibCamera::convert_rgb_to_jpeg()
        {
        }
        
        bool LibCamera::power_up()
        {
                return true;
        }
        
        bool LibCamera::power_down()
        {
                return true;
        }
        
        bool LibCamera::stand_by()
        {
                return true;
        }
        
        bool LibCamera::wake_up()
        {
                return true;
        }

        const ICameraSettings& LibCamera::get_settings()
        {
                throw std::runtime_error("LibCamera::get_settings: not implemented");
        }
}

