#include "data_provider/JsonFieldNames.h"
#include "data_provider/GpsLocationProvider.h"
#include "JsonCpp.h"

namespace romi {

    GpsLocationProvider::GpsLocationProvider(IGps &gps) : latitude_(0.0), longitude_(0.0), gps_(gps) {
    }

    std::string GpsLocationProvider::location() {

            gps_.CurrentLocation(latitude_, longitude_);

            json_object_t coordinate_object = json_object_create();
            json_object_t location_object = json_object_create();
            json_object_setnum(coordinate_object, JsonFieldNames::latitude.data(), latitude_);
            json_object_setnum(coordinate_object, JsonFieldNames::longitude.data(), longitude_);

            json_object_set(location_object, JsonFieldNames::location.data(), coordinate_object);

            std::string locationString;
            JsonCpp locationData(location_object);
            locationData.tostring(locationString, k_json_pretty);

            json_unref(location_object);
            json_unref(coordinate_object);

            return locationString;
    }

}