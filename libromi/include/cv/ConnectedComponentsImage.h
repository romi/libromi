#ifndef ROMI_ROVER_BUILD_AND_TEST_CONNECTEDCOMPONENTSIMAGE_H
#define ROMI_ROVER_BUILD_AND_TEST_CONNECTEDCOMPONENTSIMAGE_H

#include "connected.h"
#include "IConnectedComponentsImage.h"
#include "Image.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
namespace romi {
    class ConnectedComponentsImage : public IConnectedComponentsImage {

    public:
        explicit ConnectedComponentsImage(Image& image);
     //   ConnectedComponentsImage() = default;
//        ConnectedComponentsImage(uint8_t* imageData, int width, int height);
        uint8_t *do_connected(uint8_t* imageData, int width, int height);
        ~ConnectedComponentsImage() override = default;
        std::vector<uint8_t> label_data(){return label_data_;}
        unsigned char *label_data_raw(){return out_uc;}
    private:
        Image image_;
        ConnectedComponents cc_;
        std::vector<uint8_t> label_data_;
        unsigned char *out_uc;
    };
}

#pragma GCC diagnostic pop
#endif //ROMI_ROVER_BUILD_AND_TEST_CONNECTEDCOMPONENTSIMAGE_H
