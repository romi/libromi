#ifndef ROMI_ROVER_BUILD_AND_TEST_PRINCIPALCOMPONENTANALYSIS_H
#define ROMI_ROVER_BUILD_AND_TEST_PRINCIPALCOMPONENTANALYSIS_H

#include <vector>
#include <cv/IPrincipalComponentAnalysis.h>

namespace romi {
    class PrincipalComponentAnalysis : public IPrincipalComponentAnalysis {
    public:
        PrincipalComponentAnalysis() = default;

        ~PrincipalComponentAnalysis() override = default;
        void PCA(const std::vector<uint8_t>& image_mask, int channels, int width, int height,
                 double&  relative_orientation, double& cross_track_error) override;
    private:
        std::vector<Point>
        create_white_point_vector(const uint8_t *rgb_image, int width, int height, int channels);
        void component_analysis(const std::vector<Point>& white_points, Eigen::MatrixXd& center,
                                Eigen::MatrixXd& pc0_coordinates,  Eigen::MatrixXd& pc1_coordinates);
        double get_cross_track_error(int width, int height, const Eigen::MatrixXd& center,
                                     const Eigen::MatrixXd& pc0_coordinates);
        double get_sign_cte(int width, int height, const Eigen::MatrixXd& center,
                            const Eigen::MatrixXd& pc0_coordinates);
        double get_rel_orientation(const Eigen::MatrixXd& center, const Eigen::MatrixXd& pc0_coordinates);
        void try_check_pca_parameters(const std::vector<uint8_t>& image_mask, int channels, int width, int height);
    };
}

#endif //ROMI_ROVER_BUILD_AND_TEST_IPRINCIPALCOMPONENTANALYSIS_H
