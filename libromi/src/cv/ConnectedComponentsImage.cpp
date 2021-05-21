#include "cv/ConnectedComponentsImage.h"
#include "cv/connected.h"

namespace romi {
   ConnectedComponentsImage::ConnectedComponentsImage(Image& image) : image_(), cc_(30), label_data_(image.data().size()) {
        // Long hand until we sort out copy constructors in Image.
        image_ = image;
        std::vector<unsigned int> out_data(image_.width() * image_.height());
        cc_.connected(image_.export_byte_data().data(), out_data.data(), (int)image_.width(), (int)image_.height(),
                     std::equal_to<unsigned char>(),
                     constant<bool,true>());
       std::copy(out_data.data(), out_data.data() + out_data.size() , label_data_.data());
    }

    uint8_t ConnectedComponentsImage::label(int x, int y) {
       size_t offset = static_cast<size_t>((size_t)y * image_.width() + (size_t)x);
        return (label_data_[offset]);
    }


}