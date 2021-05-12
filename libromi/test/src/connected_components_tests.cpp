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

//        ImageIO::store_png(image, filename_out.c_str());
        romi::ConnectedComponentsImage cc(image);
       // auto labal_data_raw = cc.label_data_raw();
    auto labal_data = cc.label_data();


//    int stride = (int)image.width() * STBI_grey;
//    stbi_write_png("test.png", (int)image.width(), (int)image.height(), STBI_grey, labal_data_raw, stride);
//
        Image labelImage(Image::BW, labal_data.data(), image.width(), image.height());
        ImageIO::store_png(labelImage, filename_out.c_str());
}


//TEST_F(connected_component_tests, test_constructor_2)
//{
//    std::string filename("test_data/separated_mask.png");
//    std::string filename_out("label_data.png");
//    int width, height, channels = 0;
//    uint8_t* rgb_image = stbi_load(filename.c_str(), &width, &height, &channels, STBI_grey);
//
////        ImageIO::store_png(image, filename_out.c_str());
//    romi::ConnectedComponentsImage cc(30);
//    auto labal_data_raw = cc.do_connected(rgb_image, width, height);
//
//
//    int stride = (int)width * STBI_grey;
//    stbi_write_png(filename_out.c_str(), (int)width, (int)height, STBI_grey, labal_data_raw, stride);
////
////        Image labelImage(Image::BW, labal_data.data(), image.width(), image.height());
////        ImageIO::store_png(labelImage, filename_out.c_str());
//}
