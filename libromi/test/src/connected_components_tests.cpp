#include "gtest/gtest.h"
//#include "gmock/gmock.h"
#include "cv/ConnectedComponentsImage.h"
#include "cv/ImageIO.h"
//#include "cv/stb_includes.h"
#include "test_utility.h"


using namespace std;
using namespace testing;
using namespace romi;

class connected_component_tests : public ::testing::Test
{
protected:
        
	connected_component_tests() {
        }

	~connected_component_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {}
};

const std::string test_data_directory = test_utility::getexepath() + "/test_data/";

TEST_F(connected_component_tests, can_construct)
{
        std::string filename = test_data_directory + "separated_mask.png";
        std::string filename_out(test_data_directory +"label_data.png");
        Image image;
        ImageIO::load(image, filename.c_str());

        romi::ConnectedComponentsImage cc(image);
        auto labal_data = cc.label_data();

        Image labelImage(Image::BW, labal_data.data(), image.width(), image.height());
        ImageIO::store_png(labelImage, filename_out.c_str());
}

TEST_F(connected_component_tests, lmageIO_Grey_alpha)
{
    std::string filename = test_data_directory + "grey_alpha.png";
    Image image;
    ImageIO::load(image, filename.c_str());

    romi::ConnectedComponentsImage cc(image);
    auto labal_data = cc.label_data();

    Image labelImage(Image::BW, labal_data.data(), image.width(), image.height());
}