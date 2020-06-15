#include <string>
#include "gtest/gtest.h"

extern "C" {
#include "fff.h"
#include "rover.h"
}

class rover_tests : public ::testing::Test
{
protected:
	rover_tests() = default;

	~rover_tests() override = default;

	void SetUp() override
    {
	}

	void TearDown() override
    {
	}

};

TEST_F(rover_tests, new_rover_creates_new_rover)
{
    // Arrange
    // Act
    rover_t *rover = new_rover(0 , 0, 0);

    //Assert
    ASSERT_NE(rover, nullptr);
    delete_rover(rover);
}