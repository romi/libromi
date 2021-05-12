#ifndef ROMI_ROVER_BUILD_AND_TEST_CONNECTEDCOMPONENTS_H
#define ROMI_ROVER_BUILD_AND_TEST_CONNECTEDCOMPONENTS_H

#include "IConnectedComponents.h"
#include "Image.h"
namespace romi {
    class ConnectedComponents : public IConnectedComponents {

    public:
        ConnectedComponents(const Image &image);

        virtual ~ConnectedComponents() = default;

    private:
        const Image &image_;
    };
}
#endif //ROMI_ROVER_BUILD_AND_TEST_CONNECTEDCOMPONENTS_H
