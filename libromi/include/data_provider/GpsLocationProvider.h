//
// Created by douglas on 17/03/2021.
//

#ifndef ROMI_ROVER_BUILD_AND_TEST_GPSLOCATIONPROVIDER_H
#define ROMI_ROVER_BUILD_AND_TEST_GPSLOCATIONPROVIDER_H

#include "ILocationProvider.h"
#include "JsonFieldNames.h"
#include "IGPS.h"

namespace romi {

        class GpsLocationProvider : public ILocationProvider {
        public:
                explicit GpsLocationProvider(IGps &Gps);

                ~GpsLocationProvider() override = default;

                std::string get_location_string() override;
                bool update_location_estimate() override;
                v3 get_location() override;

        private:
                double startup_latitude_;
                double startup_longitude_;
                double latitude_;
                double longitude_;
                IGps &gps_;
        };
}

#endif //ROMI_ROVER_BUILD_AND_TEST_GPSLOCATIONPROVIDER_H
