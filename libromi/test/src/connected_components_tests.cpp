#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "cv/ConnectedComponents.h"

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
        Image image;
        
        ASSERT_EQ(image.width(), 0);
        ASSERT_EQ(image.height(), 0);
}

