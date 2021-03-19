#ifndef ROMI_ROVER_BUILD_AND_TEST_IIDENTITYPROVIDER_H
#define ROMI_ROVER_BUILD_AND_TEST_IIDENTITYPROVIDER_H

#include <string>
class IIdentityProvider
{
        public:
                IIdentityProvider() = default;
                virtual ~IIdentityProvider() = default;
                virtual std::string identity() = 0;
};
#endif //ROMI_ROVER_BUILD_AND_TEST_IIDENTITYPROVIDER_H
