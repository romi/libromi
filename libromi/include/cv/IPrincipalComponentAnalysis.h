#ifndef ROMI_ROVER_BUILD_AND_TEST_IPRINCIPALCOMPONENTANALYSIS_H
#define ROMI_ROVER_BUILD_AND_TEST_IPRINCIPALCOMPONENTANALYSIS_H

#include <cv/EigenIncludes.h>
#include <cv/Point.h>

namespace romi {
    class IPrincipalComponentAnalysis {
    public:
        IPrincipalComponentAnalysis() = default;

        virtual ~IPrincipalComponentAnalysis() = default;
        virtual void PCA(const std::vector<uint8_t>& image_mask, int channels, int width, int height,
                         double&  relative_orientation, double& cross_track_error) = 0;
    };
}

#endif //ROMI_ROVER_BUILD_AND_TEST_IPRINCIPALCOMPONENTANALYSIS_H
