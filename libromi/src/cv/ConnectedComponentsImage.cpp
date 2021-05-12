#include "cv/ConnectedComponentsImage.h"
#include "cv/connected.h"

namespace romi {
   ConnectedComponentsImage::ConnectedComponentsImage(Image& image) : image_(), cc_(30), label_data_(image.data().size()) , out_uc(nullptr){
  //      ConnectedComponentsImage::ConnectedComponentsImage(Image& image) : image_(), cc_(30), label_data_(), out_uc(nullptr){
        // Long hand until we sort out copy constructors in Image.
        image_ = image;
//        size_t data_size = image_.width()*image_.height();
//
//        unsigned int *out_data = (unsigned int *)malloc(sizeof(*out_data)*data_size);
//        cc_.connected(image_.export_byte_data().data(), out_data, (int)image_.width(), (int)image_.height(),
//                      std::equal_to<unsigned char>(),
//                      constant<bool,true>());
//
//        out_uc = (unsigned char *)malloc(data_size);
//        std::copy(out_data, out_data+data_size, out_uc);
//
////        std::copy(out_data, out_data+data_size, out_uc);
//
//        copy(&out_data[0], &out_data[data_size], back_inserter(label_data_));

        ///////////////////////////////////////


               std::vector<unsigned int> out_data(image_.width() * image_.height());
        cc_.connected(image_.export_byte_data().data(), out_data.data(), (int)image_.width(), (int)image_.height(),
                     std::equal_to<unsigned char>(),
                     constant<bool,true>());
        std::copy(out_data.data(), out_data.data()+image_.width() * image_.height(), label_data_.data());
    }


    uint8_t *ConnectedComponentsImage::do_connected(uint8_t* imageData, int width, int height) {
        unsigned int *out = (unsigned int *)malloc(sizeof(*out) * static_cast<unsigned long>(width) * static_cast<unsigned long>(height));
//    unsigned char *out = (unsigned char *)malloc(sizeof(*out)*width*height);

        ConnectedComponents cc1(30);
        cc1.connected(imageData, out, width, height,
                     std::equal_to<unsigned char>(),
                     constant<bool,true>());

        out_uc = (unsigned char *)malloc(static_cast<size_t>(width * height));
        std::copy(out, out+width*height, out_uc);
        return out_uc;
    }

}