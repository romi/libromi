#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock_locationprovider.h"
#include "mock_orientationprovider.h"

#include "rover/LocationTracker.h"

using namespace std;
using namespace testing;
using namespace romi;

class locationtracker_tests : public ::testing::Test
{
protected:
        MockLocationProvider location_provider;
        MockOrientationProvider orientation_provider;
        
	locationtracker_tests()
                : location_provider(),
                  orientation_provider() {
	}

	~locationtracker_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}
};

TEST_F(locationtracker_tests, constructor_doesnt_fail)
{
        ASSERT_NO_THROW(LocationTracker tracker(location_provider, orientation_provider));
}

TEST_F(locationtracker_tests, set_distance_initializes_correctly)
{
        // Arrange
        EXPECT_CALL(location_provider, update_location_estimate()).
                WillOnce(Return(true));
        EXPECT_CALL(location_provider, get_location()).
                WillOnce(Return(v3(0.0)));
        
        EXPECT_CALL(orientation_provider, update_orientation_estimate()).
                WillOnce(Return(true));
        EXPECT_CALL(orientation_provider, get_orientation()).
                WillOnce(Return(0.0));
        
        LocationTracker tracker(location_provider, orientation_provider);

        // Act
        bool success = tracker.set_distance_to_navigate(1.0);

        // Assert
        ASSERT_EQ(success, true);
        ASSERT_EQ(tracker.get_distance_to_navigate(), 1.0);
        ASSERT_EQ(tracker.get_distance_to_end(), 1.0);
        ASSERT_EQ(tracker.get_distance_from_start(), 0.0);
        ASSERT_EQ(tracker.get_cross_track_error(), 0.0);
        ASSERT_EQ(tracker.get_orientation_error(), 0.0);
}

TEST_F(locationtracker_tests, test_update_distance_estimate_1)
{
        // Arrange
        EXPECT_CALL(location_provider, update_location_estimate())
                .WillRepeatedly(Return(true));
        EXPECT_CALL(location_provider, get_location())
                .WillOnce(Return(v3(0.0)))
                .WillOnce(Return(v3(1.0, 0.0, 0.0)));
        
        EXPECT_CALL(orientation_provider, update_orientation_estimate()).
                WillRepeatedly(Return(true));
        EXPECT_CALL(orientation_provider, get_orientation())
                .WillOnce(Return(0.0))
                .WillOnce(Return(0.0));
        
        LocationTracker tracker(location_provider, orientation_provider);

        // Act
        tracker.set_distance_to_navigate(1.0);
        bool success = tracker.update_distance_estimate();

        // Assert
        ASSERT_EQ(success, true);
        ASSERT_EQ(tracker.get_distance_to_navigate(), 1.0);
        ASSERT_EQ(tracker.get_distance_to_end(), 0.0);
        ASSERT_EQ(tracker.get_distance_from_start(), 1.0);
        ASSERT_EQ(tracker.get_cross_track_error(), 0.0);
        ASSERT_EQ(tracker.get_orientation_error(), 0.0);
}

TEST_F(locationtracker_tests, test_update_distance_estimate_2)
{
        // Arrange
        EXPECT_CALL(location_provider, update_location_estimate())
                .WillRepeatedly(Return(true));
        EXPECT_CALL(location_provider, get_location())
                .WillOnce(Return(v3(0.0)))
                .WillOnce(Return(v3(0.5, 0.1, 0.0)));
        
        EXPECT_CALL(orientation_provider, update_orientation_estimate()).
                WillRepeatedly(Return(true));
        EXPECT_CALL(orientation_provider, get_orientation())
                .WillOnce(Return(0.0))
                .WillOnce(Return(0.0));
        
        LocationTracker tracker(location_provider, orientation_provider);

        // Act
        tracker.set_distance_to_navigate(1.0);
        bool success = tracker.update_distance_estimate();

        // Assert
        ASSERT_EQ(success, true);
        ASSERT_EQ(tracker.get_distance_to_navigate(), 1.0);
        ASSERT_EQ(tracker.get_distance_to_end(), 0.5);
        ASSERT_EQ(tracker.get_distance_from_start(), 0.5);
        ASSERT_EQ(tracker.get_cross_track_error(), 0.1);
        ASSERT_EQ(tracker.get_orientation_error(), 0.0);
}

TEST_F(locationtracker_tests, test_update_distance_estimate_3)
{
        // Arrange
        EXPECT_CALL(location_provider, update_location_estimate())
                .WillRepeatedly(Return(true));
        EXPECT_CALL(location_provider, get_location())
                .WillOnce(Return(v3(0.0)))
                .WillOnce(Return(v3(0.5, -0.1, 0.0)));
        
        EXPECT_CALL(orientation_provider, update_orientation_estimate()).
                WillRepeatedly(Return(true));
        EXPECT_CALL(orientation_provider, get_orientation())
                .WillOnce(Return(0.0))
                .WillOnce(Return(0.0));
        
        LocationTracker tracker(location_provider, orientation_provider);

        // Act
        tracker.set_distance_to_navigate(1.0);
        bool success = tracker.update_distance_estimate();

        // Assert
        ASSERT_EQ(success, true);
        ASSERT_EQ(tracker.get_distance_to_navigate(), 1.0);
        ASSERT_EQ(tracker.get_distance_to_end(), 0.5);
        ASSERT_EQ(tracker.get_distance_from_start(), 0.5);
        ASSERT_EQ(tracker.get_cross_track_error(), -0.1);
        ASSERT_EQ(tracker.get_orientation_error(), 0.0);
}

TEST_F(locationtracker_tests, test_update_distance_estimate_4)
{
        // Arrange
        EXPECT_CALL(location_provider, update_location_estimate())
                .WillRepeatedly(Return(true));
        EXPECT_CALL(location_provider, get_location())
                .WillOnce(Return(v3(0.0)))
                .WillOnce(Return(v3(sqrt(2.0) / 4.0, sqrt(2.0) / 4.0, 0.0)));
        
        EXPECT_CALL(orientation_provider, update_orientation_estimate()).
                WillRepeatedly(Return(true));
        EXPECT_CALL(orientation_provider, get_orientation())
                .WillOnce(Return(M_PI / 4.0))
                .WillOnce(Return(M_PI / 4.0));
        
        LocationTracker tracker(location_provider, orientation_provider);

        // Act
        tracker.set_distance_to_navigate(1.0);
        bool success = tracker.update_distance_estimate();

        // Assert
        ASSERT_EQ(success, true);
        ASSERT_EQ(tracker.get_distance_to_navigate(), 1.0);
        ASSERT_EQ(tracker.get_distance_to_end(), 0.5);
        ASSERT_EQ(tracker.get_distance_from_start(), 0.5);
        ASSERT_NEAR(tracker.get_cross_track_error(), 0.0, 0.000001);
        ASSERT_EQ(tracker.get_orientation_error(), 0.0);
}

TEST_F(locationtracker_tests, test_update_distance_estimate_5)
{
        // Arrange
        EXPECT_CALL(location_provider, update_location_estimate())
                .WillRepeatedly(Return(true));
        EXPECT_CALL(location_provider, get_location())
                .WillOnce(Return(v3(0.0)))
                .WillOnce(Return(v3(1.0, 0.0, 0.0)));
        
        EXPECT_CALL(orientation_provider, update_orientation_estimate()).
                WillRepeatedly(Return(true));
        EXPECT_CALL(orientation_provider, get_orientation())
                .WillOnce(Return(0.0))
                .WillOnce(Return(0.0));
        
        LocationTracker tracker(location_provider, orientation_provider);

        // Act
        tracker.set_distance_to_navigate(0.0);
        bool success = tracker.update_distance_estimate();

        // Assert
        ASSERT_EQ(success, true);
        ASSERT_EQ(tracker.get_distance_to_navigate(), 0.0);
        ASSERT_EQ(tracker.get_distance_to_end(), -1.0);
        ASSERT_EQ(tracker.get_distance_from_start(), 1.0);
        ASSERT_EQ(tracker.get_cross_track_error(), 0.0);
        ASSERT_EQ(tracker.get_orientation_error(), 0.0);
}

TEST_F(locationtracker_tests, test_update_distance_estimate_6)
{
        // Arrange
        EXPECT_CALL(location_provider, update_location_estimate())
                .WillRepeatedly(Return(true));
        EXPECT_CALL(location_provider, get_location())
                .WillOnce(Return(v3(0.0)))
                .WillOnce(Return(v3(-1.0, 0.0, 0.0)));
        
        EXPECT_CALL(orientation_provider, update_orientation_estimate()).
                WillRepeatedly(Return(true));
        EXPECT_CALL(orientation_provider, get_orientation())
                .WillOnce(Return(0.0))
                .WillOnce(Return(0.0));
        
        LocationTracker tracker(location_provider, orientation_provider);

        // Act
        tracker.set_distance_to_navigate(1.0);
        bool success = tracker.update_distance_estimate();

        // Assert
        ASSERT_EQ(success, true);
        ASSERT_EQ(tracker.get_distance_to_navigate(), 1.0);
        ASSERT_EQ(tracker.get_distance_to_end(), 2.0);
        ASSERT_EQ(tracker.get_distance_from_start(), -1.0);
        ASSERT_EQ(tracker.get_cross_track_error(), 0.0);
        ASSERT_EQ(tracker.get_orientation_error(), 0.0);
}

TEST_F(locationtracker_tests, test_update_distance_estimate_for_5m_travel)
{
        // Arrange
        EXPECT_CALL(location_provider, update_location_estimate())
                .WillRepeatedly(Return(true));
        EXPECT_CALL(location_provider, get_location())
                .WillOnce(Return(v3(0.0)))
                .WillOnce(Return(v3(1.0, 0.0, 0.0)));
        
        EXPECT_CALL(orientation_provider, update_orientation_estimate()).
                WillRepeatedly(Return(true));
        EXPECT_CALL(orientation_provider, get_orientation())
                .WillOnce(Return(0.0))
                .WillOnce(Return(0.0));
        
        LocationTracker tracker(location_provider, orientation_provider);

        // Act
        tracker.set_distance_to_navigate(5.0);
        bool success = tracker.update_distance_estimate();

        // Assert
        ASSERT_EQ(success, true);
        ASSERT_EQ(tracker.get_distance_to_navigate(), 5.0);
        ASSERT_EQ(tracker.get_distance_to_end(), 4.0);
        ASSERT_EQ(tracker.get_distance_from_start(), 1.0);
        ASSERT_EQ(tracker.get_cross_track_error(), 0.0);
        ASSERT_EQ(tracker.get_orientation_error(), 0.0);
}

TEST_F(locationtracker_tests, test_update_distance_estimate_for_5m_backward_travel)
{
        // Arrange
        EXPECT_CALL(location_provider, update_location_estimate())
                .WillRepeatedly(Return(true));
        EXPECT_CALL(location_provider, get_location())
                .WillOnce(Return(v3(0.0)))
                .WillOnce(Return(v3(-1.0, 0.0, 0.0)));
        
        EXPECT_CALL(orientation_provider, update_orientation_estimate()).
                WillRepeatedly(Return(true));
        EXPECT_CALL(orientation_provider, get_orientation())
                .WillOnce(Return(0.0))
                .WillOnce(Return(0.0));
        
        LocationTracker tracker(location_provider, orientation_provider);

        // Act
        tracker.set_distance_to_navigate(-5.0);
        bool success = tracker.update_distance_estimate();

        // Assert
        ASSERT_EQ(success, true);
        ASSERT_EQ(tracker.get_distance_to_navigate(), -5.0);
        ASSERT_EQ(tracker.get_distance_to_end(), 4.0);
        ASSERT_EQ(tracker.get_distance_from_start(), 1.0);
        ASSERT_EQ(tracker.get_cross_track_error(), 0.0);
        ASSERT_EQ(tracker.get_orientation_error(), 0.0);
}

