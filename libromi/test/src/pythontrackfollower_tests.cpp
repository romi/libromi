#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <r.h>
#include "../mock/mock_camera.h"
#include "../mock/mock_rpcclient.h"
#include "../mock/mock_session.h"
#include "rover/PythonTrackFollower.h"

using namespace std;
using namespace testing;
using namespace romi;

class pythontrackfollower_tests : public ::testing::Test
{
protected:
        MockCamera camera_;
        MockRPCClient rpc_;
        MockSession session_;
        
	pythontrackfollower_tests()
                : camera_(),
                  rpc_(),
                  session_() {
                std::unique_ptr<romi::IRPCClient> rpc_;

	}

	~pythontrackfollower_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}
};

TEST_F(pythontrackfollower_tests, constructor_with_good_values_succeeds)
{
        // Arrange
        std::unique_ptr<MockRPCClient> mock_rpc = std::make_unique<MockRPCClient>();
        std::unique_ptr<romi::IRPCClient> rpc = std::move(mock_rpc);

        // Act
        PythonTrackFollower follower(camera_, rpc, "toto", session_);
}

static rpp::MemBuffer buffer;

ACTION(throw_grab_jpeg_exception)
{
        throw std::runtime_error("grab_jpeg_exception");
        return buffer;
}

TEST_F(pythontrackfollower_tests, update_error_estimate_fails_when_camera_fails)
{
        // Arrange
        std::unique_ptr<MockRPCClient> mock_rpc = std::make_unique<MockRPCClient>();

        EXPECT_CALL(camera_, grab_jpeg)
                .WillOnce(throw_grab_jpeg_exception());

        std::unique_ptr<romi::IRPCClient> rpc = std::move(mock_rpc);
        
        PythonTrackFollower follower(camera_, rpc, "toto", session_);

        // Act
        bool success = follower.update_error_estimate();
        
        // Assert
        ASSERT_FALSE(success);
}
