#include "test_utility.h"
#include <limits.h>
#include <unistd.h>
#include <filesystem>

std::string test_utility::getexepath()
{
    char result[ PATH_MAX ];
    std::string pstring;
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );

    std::string sresult(result, (count > 0) ? (size_t)count : 0 );
    pstring = std::filesystem::path(result).parent_path();
    return pstring;
}