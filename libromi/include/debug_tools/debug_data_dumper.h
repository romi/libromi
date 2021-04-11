#ifndef ROMI_ROVER_BUILD_AND_TEST_DEBUG_DATA_DUMPER_H
#define ROMI_ROVER_BUILD_AND_TEST_DEBUG_DATA_DUMPER_H

#include <filesystem>
#include <fstream>
#include <iostream>

#ifndef NDEBUG

const int MAX_NAME_LEN = 31;
#define OPEN_DUMP(x) debug_data_dumper::open_dump(x)
#define DUMP(n,r,c,v) debug_data_dumper::dump(n,r,c,v)
#define DUMP_INTERLEAVE(n,s,a,b) debug_data_dumper::dump_interleave(n,s,a,b)
#define CLOSE_DUMP() debug_data_dumper::close_dump()

class debug_data_dumper {
public:
    static void open_dump(const std::filesystem::path& dump_directory );
    static void close_dump();

    template<typename T>
    static void dump(const std::string& name, int32_t rows, int32_t cols, T *values) {
        size_t name_len = (name.length() > MAX_NAME_LEN) ? MAX_NAME_LEN : name.length();
        if (out_.is_open())
        {
                out_.write((const char*)&name_len, sizeof(int32_t));
                out_.write(name.data(), static_cast<long>(name_len));
                out_.write((const char*)&rows, sizeof(int32_t));
                out_.write((const char*)&cols, sizeof(int32_t));
                int index = 0;
                for (int row = 0; row < rows; row++) {
                        for (int col = 0; col < cols; col++) {
                                auto value = static_cast<double>(values[index]);
                                // Numbers are always double size to keep file compatibility.
                                out_.write((const char*)&value, sizeof(double));
                                index++;
                        }
                }
        }
    }

    template<typename T>
    static void dump_interleave(const std::string& name, int32_t size, T *a, T *b)
    {
            const int32_t columns = 2;
            size_t name_len = (name.length() > MAX_NAME_LEN) ? MAX_NAME_LEN : name.length();
            if (out_.is_open()) {
                    out_.write((const char *) &name_len, sizeof(int32_t));
                    out_.write(name.data(), static_cast<long>(name_len));
                    out_.write((const char *) &size, sizeof(int32_t));
                    out_.write((const char *) &columns, sizeof(int32_t));
                    for (int i = 0; i < size; i++) {
                            auto value_a = static_cast<double>(a[i]);
                            auto value_b = static_cast<double>(b[i]);
                            out_.write((const char*)&value_a, sizeof(double));
                            out_.write((const char*)&value_b, sizeof(double));
                    }
            }
    }

private:
    static std::ofstream out_;

};

#else
#define OPEN_DUMP(x)
#define DUMP(n,r,c,v)
#define DUMP_INTERLEAVE(n,s,a,b)
#define CLOSE_DUMP()
#endif
#endif //ROMI_ROVER_BUILD_AND_TEST_DEBUG_DATA_DUMPER_H
