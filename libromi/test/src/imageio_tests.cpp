#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "cv/ImageIO.h"
#include <FileUtils.h>
#include "test_utility.h"

using namespace std;
using namespace testing;
using namespace romi;

const std::string test_data_directory = test_utility::getexepath() + "/test_data/";

uint8_t grey[] = { 0,  32,  64,  96,
                   32, 64,  96,  128,
                   64, 96,  128, 160,
                   96, 128, 160, 192 };

uint8_t red[] = { 0,0,0,  32,0,0,  64,0,0,  96,0,0,
                  32,0,0, 64,0,0,  96,0,0,  128,0,0,
                  64,0,0, 96,0,0,  128,0,0, 160,0,0,
                  96,0,0, 128,0,0, 160,0,0, 192,0,0 };

class imageio_tests : public ::testing::Test
{
protected:

        static constexpr const char *jpg_file = "/tmp/imageio_tests.jpg";
        static constexpr const char *png_file = "/tmp/imageio_tests.png";
        
        Image bw;
        Image rgb;
        
	imageio_tests() : bw(), rgb()
	{}

	~imageio_tests() override = default;

	void SetUp() override {
                bw.import(Image::BW, grey, 4, 4);
                rgb.import(Image::RGB, red, 4, 4);
        }

	void TearDown() override {}
};

TEST_F(imageio_tests, store_jpg_returns_error_on_invalid_file)
{
        bool success = ImageIO::store_jpg(bw, "/foo/bar");
        ASSERT_EQ(success, false);
}

TEST_F(imageio_tests, successful_store_jpg)
{
        bool success = ImageIO::store_jpg(bw, jpg_file);
        ASSERT_EQ(success, true);
}

// NOTE: STB only saves jpeg in 3 channels.
// Save as 1 channel BW load as 3 channel RGB.
TEST_F(imageio_tests, successful_store_and_load_jpg_BW)
{
        bool success = ImageIO::store_jpg(bw, jpg_file);
        ASSERT_EQ(success, true);

        Image image;
        success = ImageIO::load(image, jpg_file);
        ASSERT_EQ(success, true);
        ASSERT_EQ(image.type(), Image::RGB);
        ASSERT_EQ(image.width(), 4);
        ASSERT_EQ(image.height(), 4);
        ASSERT_EQ(image.channels(), 3);

}

TEST_F(imageio_tests, successful_store_and_load_jpg_RGB)
{
        bool success = ImageIO::store_jpg(rgb, jpg_file);
        ASSERT_EQ(success, true);

        Image image;
        success = ImageIO::load(image, jpg_file);

        ASSERT_EQ(success, true);
        ASSERT_EQ(image.type(), Image::RGB);
        ASSERT_EQ(image.width(), 4);
        ASSERT_EQ(image.height(), 4);
        ASSERT_EQ(image.channels(), 3);
        ASSERT_EQ(rgb.data().size(), image.data().size());
}

TEST_F(imageio_tests, load_returns_false_when_file_doesnt_exist)
{
    // Arrange
    std::string filename = test_data_directory + "rgba_not_exist.png";
    Image image;

    //Act
    auto actual = ImageIO::load(image, filename.c_str());

    //Assert
    ASSERT_FALSE(actual);
    ASSERT_EQ(image.type(), Image::RGB);

}

TEST_F(imageio_tests, load_jpg_from_buffer_correct)
{
        bool success = ImageIO::store_jpg(rgb, jpg_file);
        ASSERT_EQ(success, true);

        std::vector<uint8_t> inbuffer;
        FileUtils::TryReadFileAsVector(jpg_file, inbuffer);

        Image image_from_buffer;
        success = ImageIO::load_from_buffer(image_from_buffer, inbuffer);

        Image image_from_file;
        success = ImageIO::load(image_from_file, jpg_file);

        ASSERT_EQ(success, true);
        ASSERT_EQ(image_from_buffer.data().size(), image_from_file.data().size());
        ASSERT_EQ(image_from_buffer.data(), image_from_file.data());
}

TEST_F(imageio_tests, load_rgba_from_buffer_loads_as_rgb)
{

    std::string filename = test_data_directory + "rgba.png";
    std::vector<uint8_t> inbuffer;
    FileUtils::TryReadFileAsVector(filename.c_str(), inbuffer);

    Image image_from_buffer;
    auto success = ImageIO::load_from_buffer(image_from_buffer, inbuffer);

    Image image_from_file;
    success = ImageIO::load(image_from_file, filename.c_str());

    ASSERT_EQ(success, true);
    ASSERT_EQ(image_from_buffer.type(), Image::RGB);
    ASSERT_EQ(image_from_buffer.data().size(), image_from_file.data().size());
    ASSERT_EQ(image_from_buffer.data(), image_from_file.data());
}

TEST_F(imageio_tests, load_grey_alpha_from_buffer_loads_as_grey)
{

    std::string filename = test_data_directory + "grey_alpha.png";
    std::vector<uint8_t> inbuffer;
    FileUtils::TryReadFileAsVector(filename.c_str(), inbuffer);

    Image image_from_buffer;
    auto success = ImageIO::load_from_buffer(image_from_buffer, inbuffer);

    Image image_from_file;
    success = ImageIO::load(image_from_file, filename.c_str());

    ASSERT_EQ(success, true);
    ASSERT_EQ(image_from_buffer.type(), Image::BW);
    ASSERT_EQ(image_from_buffer.data().size(), image_from_file.data().size());
    ASSERT_EQ(image_from_buffer.data(), image_from_file.data());
}

TEST_F(imageio_tests, load_from_buffer_fails)
{
    std::vector<uint8_t> inbuffer;

    Image image_from_buffer;
    auto success = ImageIO::load_from_buffer(image_from_buffer, inbuffer);

    ASSERT_EQ(success, false);
}

TEST_F(imageio_tests, store_png_returns_error_on_invalid_file)
{
        bool success = ImageIO::store_png(bw, "/foo/bar");
        ASSERT_EQ(success, false);
}

TEST_F(imageio_tests, successful_store_png)
{
        bool success = ImageIO::store_png(bw, png_file);
        ASSERT_EQ(success, true);
}

TEST_F(imageio_tests, successful_store_and_load_png_BW)
{
        // Arrange

        // Act
        bool success = ImageIO::store_png(bw, png_file);
        ASSERT_EQ(success, true);

        Image image;
        success = ImageIO::load(image, png_file);

        // Asert
        ASSERT_EQ(success, true);
        ASSERT_EQ(image.type(), Image::BW);
        ASSERT_EQ(image.width(), 4);
        ASSERT_EQ(image.height(), 4);
        ASSERT_EQ(image.channels(), 1);

        auto& p0 = bw.data();
        auto& p1 = image.data();
        for (size_t i = 0; i < image.length(); i++) {
                ASSERT_NEAR(p0[i], p1[i], 0.004);
        }
}

TEST_F(imageio_tests, successful_store_and_load_png_RGB)
{
        bool success = ImageIO::store_png(rgb, png_file);
        ASSERT_EQ(success, true);

        Image image;
        //Act
        auto actual = ImageIO::load(image, png_file);

        //Assert
        ASSERT_TRUE(actual);
        ASSERT_EQ(image.type(), Image::RGB);
        ASSERT_EQ(image.width(), 4);
        ASSERT_EQ(image.height(), 4);
        ASSERT_EQ(image.channels(), 3);

        auto& p0 = rgb.data();
        auto& p1 = image.data();
        for (size_t i = 0; i < image.length(); i++) {
                ASSERT_NEAR(p0[i], p1[i], 0.004);
        }
}

TEST_F(imageio_tests, grey_alpha_loaded_as_grey)
{
    std::string filename = test_data_directory + "grey_alpha.png";
    Image image;
    //Act
    auto actual = ImageIO::load(image, filename.c_str());

    //Assert
    ASSERT_TRUE(actual);

    ASSERT_EQ(image.type(), Image::BW);

}

TEST_F(imageio_tests, rgb_alpha_loaded_as_rgb)
{
    std::string filename = test_data_directory + "rgba.png";
    Image image;
    //Act
    auto actual = ImageIO::load(image, filename.c_str());

    //Assert
    ASSERT_TRUE(actual);

    ASSERT_EQ(image.type(), Image::RGB);

}

