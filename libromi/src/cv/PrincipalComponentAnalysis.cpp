#include "cv/PrincipalComponentAnalysis.h"
#include <Eigen/Eigenvalues>


namespace romi {

    uint8_t
    GetPixel(const uint8_t * image, int x, int y, int numchannels, int width)
    {

        std::tuple<uint8_t, uint8_t, uint8_t> pixel;
        const uint8_t *p = image + (numchannels * (y * width + x));
        return p[0];
    }

    std::vector<Point>
    PrincipalComponentAnalysis::create_white_point_vector(const uint8_t *rgb_image, const int width, const int height, const int numchannels) {
        std::vector<Point> vector_white_point;
        for (int rows = 0; rows < height; rows++) {
            for (int cols = 0; cols < width; ++cols) {
                auto pixel = GetPixel(rgb_image, cols, rows, numchannels, width);
                if (pixel == 255) {
                    vector_white_point.emplace_back(cols, rows);
                }
            }
        }
        return vector_white_point;
    }

    void PrincipalComponentAnalysis::component_analysis(const std::vector<Point>& white_points, Eigen::MatrixXd& center,
                                                        Eigen::MatrixXd& pc0_coordinates,  Eigen::MatrixXd& pc1_coordinates)
    {
        Eigen::MatrixXd white_point_matrix( white_points.size(), 2);
        int row = 0;
        for (auto& point: white_points) {
            // XY inverted as we are looking up->down not left->right in image.
            Eigen::Vector2d vec(point.y,point.x);
            white_point_matrix.row(row++) = vec;
        }

        auto center_mean = white_point_matrix.colwise().mean();
        Eigen::MatrixXd centered_points = white_point_matrix.rowwise() - center_mean;

        Eigen::MatrixXd cov = (centered_points.adjoint() * centered_points)/ double(white_point_matrix.rows());
        Eigen::JacobiSVD<Eigen::MatrixXd> svd(cov, Eigen::ComputeThinU | Eigen::ComputeThinV);

        double svd0sqrt = std::sqrt(svd.singularValues()[0]);
        double svd1sqrt =  std::sqrt(svd.singularValues()[1]);
        pc0_coordinates = center_mean.transpose() + svd.matrixU().col(0) * svd0sqrt;
        pc1_coordinates = center_mean.transpose() + svd.matrixU().col(1) * svd1sqrt;
        center = center_mean;
    }

    double PrincipalComponentAnalysis::get_sign_cte(int width, int height, const Eigen::MatrixXd& center,
                                                    const Eigen::MatrixXd& pc0_coordinates)
    {
        auto center2d = center.data();
        auto pc02d = pc0_coordinates.data();
        double a = -(pc02d[1]-center2d[1])/(pc02d[0]-center2d[0]);
        double c = -(a*center2d[0]+center2d[1]);
        double sign = a*height+width+c;
        if (sign >= 0) return 1;
        else return -1;
    }

    double PrincipalComponentAnalysis::get_cross_track_error(int width, int height, const Eigen::MatrixXd& center,
                                                             const Eigen::MatrixXd& pc0_coordinates)
    {
        double sign = get_sign_cte(width, height, center, pc0_coordinates);
        auto center2d = center.data();
        auto pc02d = pc0_coordinates.data();

        double abs_dims = std::abs(
                (pc02d[0]-center2d[0])*(center2d[1]-width)-(center2d[0]-height)*(pc02d[1]-center2d[1])
        );
        double sqrt_dims = std::sqrt(
                std::pow((pc02d[0]-center2d[0]),2) + std::pow(pc02d[1]-center2d[1], 2)
        );

        double cte = sign * (abs_dims / sqrt_dims);
        return cte;
    }

    double PrincipalComponentAnalysis::get_rel_orientation(const Eigen::MatrixXd& center,
                                                           const Eigen::MatrixXd& pc0_coordinates)
    {
        std::vector<double> center2d(center.data(), center.data() + center.size());
        std::vector<double> pc02d(pc0_coordinates.data(), pc0_coordinates.data() + pc0_coordinates.size());
        std::vector<std::vector<double>> pts = {pc02d, center2d};

        std::sort(pts.begin(), pts.end(),
                  [](const std::vector<double>& a, const std::vector<double>& b) {
                      return a[0] < b[0];
                  });

        double y = pts[0][0]-pts[1][0];
        double x = pts[0][1]-pts[1][1];
        double atan_res = std::atan2(y, x);
        double pi_res = (M_PI / 2);
        double res =  atan_res + pi_res;
        return res;
    }

    void
    PrincipalComponentAnalysis::try_check_pca_parameters(const std::vector<uint8_t> &image_mask, int channels, int width,
                                                     int height)
    {   std::string error("try_check_pca_parameters: ");
        if (image_mask.empty())
            throw std::runtime_error(error + "empty image mask");
        if (channels != 1)
            throw std::runtime_error(error + "only B&W images supported. channels: " + std::to_string(channels));
        if ( width <= 0 || height <= 0)
            throw std::runtime_error(error + "invalid image dimensions: width: " + std::to_string(width)
            + " height: " + std::to_string(height));
    }

    void
    PrincipalComponentAnalysis::PCA(const std::vector<uint8_t>& image_mask, int channels, int width, int height,
                                          double& relative_orientation,
                                          double& cross_track_error)
    {
        try_check_pca_parameters(image_mask, channels, width, height);
        Eigen::MatrixXd pc0_coordinates;
        Eigen::MatrixXd pc1_coordinates;
        Eigen::MatrixXd center;
        auto white_pixels_points = create_white_point_vector(image_mask.data(), width, height, channels);
        component_analysis(white_pixels_points, center, pc0_coordinates, pc1_coordinates);
        cross_track_error = get_cross_track_error(width/2, height/2, center, pc0_coordinates);
        relative_orientation = get_rel_orientation(center, pc0_coordinates);
    }

}