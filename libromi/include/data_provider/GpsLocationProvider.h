//
// Created by douglas on 17/03/2021.
//

#ifndef ROMI_ROVER_BUILD_AND_TEST_GPSLOCATIONPROVIDER_H
#define ROMI_ROVER_BUILD_AND_TEST_GPSLOCATIONPROVIDER_H

#include "ILocationProvider.h"
#include "JsonFieldNames.h"
#include "IGPS.h"

class GpsLocationProvider : public ILocationProvider {
        public:
                explicit GpsLocationProvider(IGps& Gps);
                ~GpsLocationProvider() override = default;
                std::string location() override;
        private:
                double latitude_;
                double longitude_;
                IGps& gps_;
};


#endif //ROMI_ROVER_BUILD_AND_TEST_GPSLOCATIONPROVIDER_H
