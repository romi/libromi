#include "data_provider/JsonFieldNames.h"
#include "data_provider/GpsLocationProvider.h"
#include "JsonCpp.h"

namespace romi {

        GpsLocationProvider::GpsLocationProvider(IGps &gps)
                : startup_latitude_(0.0),
                  startup_longitude_(0.0),
                  latitude_(0.0),
                  longitude_(0.0),
                  gps_(gps) {
                gps_.CurrentLocation(startup_longitude_, startup_longitude_);
        }

        bool GpsLocationProvider::update_location_estimate()
        {
                gps_.CurrentLocation(latitude_, longitude_);
                return true;
        }
        
        std::string GpsLocationProvider::get_location_string()
        {
                if (!update_location_estimate()) {
                        r_warn("GpsLocationProvider: update failed. "
                               "Returning old estimates");
                }

                json_object_t coordinate_object = json_object_create();
                json_object_setnum(coordinate_object, JsonFieldNames::latitude.data(),
                                   latitude_);
                json_object_setnum(coordinate_object, JsonFieldNames::longitude.data(),
                                   longitude_);
                std::string locationString;
                JsonCpp locationData(coordinate_object);
                locationData.tostring(locationString, k_json_pretty | k_json_sort_keys);

                json_unref(coordinate_object);
                return locationString;
        }

        v3 GpsLocationProvider::get_location()
        {
                throw std::runtime_error("GpsLocationProvider::get_location: "
                                         "NOT IMPLEMENTED YET");
        }
}
