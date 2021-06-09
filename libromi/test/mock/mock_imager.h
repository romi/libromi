#include "gmock/gmock.h"
#include "api/IImager.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockImager : public romi::IImager
{
public:
        MOCK_METHOD(bool, start_recording, (const std::string& observation_id,
                                            size_t max_images,
                                            double max_duration), (override));
        MOCK_METHOD(bool, stop_recording, (), (override));

        MOCK_METHOD(bool, is_recording, (), (override));
};
#pragma GCC diagnostic pop
