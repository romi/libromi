#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GetOpt.h"

using namespace std;
using namespace testing;
using namespace romi;

class roveroptions_tests : public ::testing::Test
{
protected:
        
	roveroptions_tests() {
        }

	~roveroptions_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {}
};

TEST_F(roveroptions_tests, test_parse)
{
        std::vector<Option> options_list = {
                { "help", false, nullptr, "Print help message" },
                { "config", true, nullptr, "Path of the config file" },
        };
        GetOpt options(options_list);

        const char *argv[] = { "app", "--help", "--config", "config.json", nullptr };

        options.parse(4, (char**) argv);

        ASSERT_EQ(options.is_help_requested(), true);
        ASSERT_STREQ(options.get_value("config"), "config.json");
}

TEST_F(roveroptions_tests, test_print_usage)
{
        std::vector<Option> options_list = {
                { "help", false, nullptr, "Print help message" },
                { "config", true, nullptr, "Path of the config file" },
        };
        GetOpt options(options_list);
        options.print_usage();
}

