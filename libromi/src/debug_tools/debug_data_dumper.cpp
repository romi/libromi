#include "debug_tools/debug_data_dumper.h"

#ifndef NDEBUG

std::ofstream debug_data_dumper::out_;
void debug_data_dumper::open_dump(const std::filesystem::path& dump_path)
{
        close_dump();
        out_.open(dump_path, std::ios::out | std::ios::binary);
        out_.exceptions(std::ofstream::badbit | std::ofstream::failbit);
}

void debug_data_dumper::close_dump()
{
        if(out_.is_open())
                out_.close();
}

#endif