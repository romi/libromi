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
        ~ConnectedComponentsImage() override = default;
        uint8_t label(int x, int y);

        std::vector<uint8_t> label_data(){return label_data_;}
    private:
        Image image_;
        ConnectedComponents cc_;
        std::vector<uint8_t> label_data_;
    };
}

#pragma GCC diagnostic pop
#endif //ROMI_ROVER_BUILD_AND_TEST_CONNECTEDCOMPONENTSIMAGE_H
