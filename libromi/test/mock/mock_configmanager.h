#include "gmock/gmock.h"
#include "api/IConfigManager.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

class MockConfigManager : public romi::IConfigManager
{
public:
        MOCK_METHOD(bool, has_section, (const std::string& name), (override));
        
        MOCK_METHOD(void, set_section, (const std::string& name, nlohmann::json& value), (override));
        MOCK_METHOD(nlohmann::json, get_section, (const std::string& name), (override));
        MOCK_METHOD(nlohmann::json, get, (), (override));
};
#pragma GCC diagnostic pop
