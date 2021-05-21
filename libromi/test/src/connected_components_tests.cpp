#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "cv/cv.h"
#include "cv/ImageIO.h"
#include "FileUtils.h"
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
        // Arrange
        std::string filename = test_data_directory + "separated_mask.png";
        std::string filename_reference(test_data_directory +"connected_component_reference_image.png");
        Image mask;
        Image components;
        Image referenceComponent;

        // Act
        if (ImageIO::load(mask, filename.c_str()))
        {
            compute_connected_components(mask, components);
            ImageIO::load(referenceComponent, filename_reference.c_str());
        }

        // Assert
        ASSERT_EQ(components.export_byte_data(), referenceComponent.export_byte_data());
}
