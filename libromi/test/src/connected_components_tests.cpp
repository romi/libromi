#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "cv/ConnectedComponentsImage.h"
#include "cv/ImageIO.h"
#include "cv/stb_includes.h"


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

TEST_F(connected_component_tests, test_constructor_1)
{
        std::string filename("test_data/separated_mask.png");
        std::string filename_out("label_data.png");
        Image image;
        ImageIO::load(image, filename.c_str());

        romi::ConnectedComponentsImage cc(image);
        auto labal_data = cc.label_data();

        Image labelImage(Image::BW, labal_data.data(), image.width(), image.height());
        ImageIO::store_png(labelImage, filename_out.c_str());
}

