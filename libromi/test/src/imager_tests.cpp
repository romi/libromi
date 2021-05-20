#include <chrono>
#include "gtest/gtest.h"

#include <mock_camera.h>
#include <mock_session.h>
#include <camera/Imager.h>

using namespace testing;

class imager_tests : public ::testing::Test {
protected:
        rpp::MemBuffer buffer_;
        
        imager_tests() : buffer_() {}
        
        ~imager_tests() override = default;
        
        void SetUp() override {
        }
        
        void TearDown() override {
        }
};

TEST_F(imager_tests, constructor_succeeds)
{
        // Arrange
        MockSession session;
        MockCamera camera;

        // Act & Assert
        ASSERT_NO_THROW(romi::Imager imager(session, camera));
}

TEST_F(imager_tests, record_one_image)
{ 
        // Arrange
        MockSession session;
        MockCamera camera;
        romi::Imager imager(session, camera);
        buffer_.printf("not empty");
        
        EXPECT_CALL(session, start("foo"));
        EXPECT_CALL(camera, grab_jpeg())
                .WillOnce(ReturnRef(buffer_));
        EXPECT_CALL(session, store_jpg(_, An<rpp::MemBuffer&>()))
                .WillOnce(Return(true));
        EXPECT_CALL(session, stop());
        
        // Act
        imager.start_recording("foo", 1, 0.1);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        imager.stop_recording();

        // Assert
}
