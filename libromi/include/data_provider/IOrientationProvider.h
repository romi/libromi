#ifndef ROMI_IORIENTATIONPROVIDER_H
#define ROMI_IORIENTATIONPROVIDER_H

#include <string>

namespace romi {

    class IOrientationProvider {
    public:
        IOrientationProvider() = default;
        virtual ~IOrientationProvider() = default;
        
        virtual bool update_orientation_estimate() = 0;
        virtual double get_orientation() = 0;
    };

}
#endif // ROMI_IORIENTATIONPROVIDER_H
