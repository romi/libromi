#include "data_provider/SoftwareVersion.h"

namespace romi {

    std::string SoftwareVersion::SoftwareVersionCurrent() {
            return std::string("0.1RC1");
    }

    std::string SoftwareVersion::SoftwareVersionAlternate() {
            return std::string("N/A");
    }

}