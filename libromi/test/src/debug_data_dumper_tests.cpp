#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "debug_tools/debug_data_dumper.h"
#include "FileUtils.h"

using namespace std;
using namespace testing;

// These tests are now obsolete as they were used to compare the old dump code with the new templated code.
// They will now always pass (in debug mode)
// They are not included as part of the normak test run but are left here for reference.
class debug_data_dumper_tests : public ::testing::Test
{
protected:
        
	debug_data_dumper_tests() : dump_path_("./dump"){
        }

	~debug_data_dumper_tests() override = default;

	void SetUp() override {
	        std::filesystem::remove_all(dump_path_);
	        std::filesystem::create_directories(dump_path_);
        }

	void TearDown() override {}
	std::filesystem::path dump_path_;
};

TEST_F(debug_data_dumper_tests, can_open_close_dump)
{
        // Arrange
        // Act
        OPEN_DUMP(dump_path_/"dump.out");
        CLOSE_DUMP();

        // Assert
        ASSERT_TRUE(std::filesystem::exists(dump_path_/"dump.out"));
}


TEST_F(debug_data_dumper_tests, dump_row_col_float_array_dumps_correctly)
{
        // Arrange
        float float_data[] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f};

                              // Act
        OPEN_DUMP(dump_path_/"dump.out");
        DUMP("float_data", 4, 2, float_data);
        CLOSE_DUMP();

        // Assert
        ASSERT_TRUE(std::filesystem::exists(dump_path_/"dump.out"));
}

TEST_F(debug_data_dumper_tests, dump_row_col_float_array_old_new_functions_match_output)
{
        // Arrange
        float data[] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f};

        // Act
        OPEN_DUMP(dump_path_/"dump.out");
        DUMP("float_data", 4, 2, data);
        CLOSE_DUMP();

        debug_data_dumper::open_dump(dump_path_/"dump_template.out");
        debug_data_dumper::dump("float_data", 4, 2, data);
        debug_data_dumper::close_dump();


        std::vector<uint8_t> original_output;
        std::vector<uint8_t> template_output;

        FileUtils::TryReadFileAsVector(dump_path_/"dump.out", original_output);
        FileUtils::TryReadFileAsVector(dump_path_/"dump_template.out", template_output);

        // Assert
        ASSERT_EQ(original_output, template_output);
}

TEST_F(debug_data_dumper_tests, dump_row_col_double_array_old_new_functions_match_output)
{
        // Arrange
        double data[] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f};

        // Act
        OPEN_DUMP(dump_path_/"dump.out");
        DUMP("float_data", 4, 2, data);
        CLOSE_DUMP();

        debug_data_dumper::open_dump(dump_path_/"dump_template.out");
        debug_data_dumper::dump("float_data", 4, 2, data);
        debug_data_dumper::close_dump();


        std::vector<uint8_t> original_output;
        std::vector<uint8_t> template_output;

        FileUtils::TryReadFileAsVector(dump_path_/"dump.out", original_output);
        FileUtils::TryReadFileAsVector(dump_path_/"dump_template.out", template_output);

        // Assert
        ASSERT_EQ(original_output, template_output);
}

TEST_F(debug_data_dumper_tests, dump_row_col_double_array_very_long_name_matches_output)
{
        // Arrange
        double data[] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f};
        std::string data_name("A very long name with more than 31 characters");
        std::string original_filename("dump.out");
        std::string template_filename("dump_template.out");
        // Act
        OPEN_DUMP(dump_path_/original_filename);
        DUMP(data_name.c_str(), 4, 2, data);
        CLOSE_DUMP();

        debug_data_dumper::open_dump(dump_path_/template_filename);
        debug_data_dumper::dump(data_name, 4, 2, data);
        debug_data_dumper::close_dump();


        std::vector<uint8_t> original_output;
        std::vector<uint8_t> template_output;

        FileUtils::TryReadFileAsVector(dump_path_/"dump.out", original_output);
        FileUtils::TryReadFileAsVector(dump_path_/"dump_template.out", template_output);

        // Assert
        ASSERT_EQ(original_output, template_output);
}

TEST_F(debug_data_dumper_tests, dump_interleave_double_array_matches_output)
{
        // Arrange
        const int size = 8;
        double data_a[size] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f};
        double data_b[size] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f};
        std::string data_name("A very long name with more than 31 characters");
        std::string original_filename("dump.out");
        std::string template_filename("dump_template.out");
        // Act
        OPEN_DUMP(dump_path_/original_filename);
        DUMP_INTERLEAVE(data_name.c_str(), size, data_a, data_b);
        CLOSE_DUMP();

        debug_data_dumper::open_dump(dump_path_/template_filename);
        debug_data_dumper::dump_interleave(data_name, size, data_a, data_b);
        debug_data_dumper::close_dump();


        std::vector<uint8_t> original_output;
        std::vector<uint8_t> template_output;

        FileUtils::TryReadFileAsVector(dump_path_/"dump.out", original_output);
        FileUtils::TryReadFileAsVector(dump_path_/"dump_template.out", template_output);

        // Assert
        ASSERT_EQ(original_output, template_output);
}

TEST_F(debug_data_dumper_tests, dump_interleave_float_array_matches_output)
{
        // Arrange
        const int size = 8;
        float data_a[size] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f};
        float data_b[size] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f};
        std::string data_name("A very long name with more than 31 characters");
        std::string original_filename("dump.out");
        std::string template_filename("dump_template.out");
        // Act
        OPEN_DUMP(dump_path_/original_filename);
        DUMP_INTERLEAVE(data_name.c_str(), size, data_a, data_b);
        CLOSE_DUMP();

        debug_data_dumper::open_dump(dump_path_/template_filename);
        debug_data_dumper::dump_interleave(data_name, size, data_a, data_b);
        debug_data_dumper::close_dump();


        std::vector<uint8_t> original_output;
        std::vector<uint8_t> template_output;

        FileUtils::TryReadFileAsVector(dump_path_/"dump.out", original_output);
        FileUtils::TryReadFileAsVector(dump_path_/"dump_template.out", template_output);

        // Assert
        ASSERT_EQ(original_output, template_output);
}