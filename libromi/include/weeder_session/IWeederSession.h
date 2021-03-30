#ifndef ROMI_ROVER_BUILD_AND_TEST_IWEEDERSESSION_H
#define ROMI_ROVER_BUILD_AND_TEST_IWEEDERSESSION_H

#include <string>

namespace romi {

    class IWeederSession {
    public:
        IWeederSession() = default;
        virtual ~IWeederSession() = default;
        virtual void Start(const std::string &observation_id) = 0;
        virtual void Stop() = 0;
    };

}

#endif //ROMI_ROVER_BUILD_AND_TEST_WEEDERSESSION_H
