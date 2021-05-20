#include "gmock/gmock.h"
#include "session/ISession.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockSession : public romi::ISession
{
public:
        MOCK_METHOD(void, start, (const std::string& observation_id), (override));
        MOCK_METHOD(void, stop, (), (override));
        MOCK_METHOD(bool, store_jpg, (const std::string& name, romi::Image& image), (override));
        MOCK_METHOD(bool, store_jpg, (const std::string& name, rpp::MemBuffer& jpeg), (override));
        MOCK_METHOD(bool, store_png, (const std::string& name, romi::Image& image) , (override));
        MOCK_METHOD(bool, store_svg, (const std::string& name, const std::string& body), (override));
        MOCK_METHOD(bool, store_txt, (const std::string& name, const std::string& body), (override));
        MOCK_METHOD(bool, store_path, (const std::string& filename, int32_t path_number, romi::Path& weeder_path), (override));
        MOCK_METHOD(std::filesystem::path, current_path, (), (override));
};
#pragma GCC diagnostic pop
