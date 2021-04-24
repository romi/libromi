/*
  romi-rover

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  romi-rover is collection of applications for the Romi Rover.

  romi-rover is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */

#ifndef __ROMI_GETOPT_H
#define __ROMI_GETOPT_H

#include <string>
#include <vector>
#include <map>
#include <getopt.h>
#include "configuration/IOptions.h"

namespace romi {

        class Option
        {
        public:
                const char *name;
                bool requires_value;
                const char *default_value;
                const char *description;
        };
        
        class GetOpt : public IOptions
        {
        protected:
                std::vector<Option> _options;
                std::vector<option> _long_options;
                std::vector<std::string> _descriptions;
                std::map<std::string, bool> _flags;
                std::map<std::string, std::string> _values;

                void generate_long_options();
                void append_long_option(Option& option);
                void append_zero_option();
                void generate_descriptions();
                void set_option(size_t index, const char *value);
                std::string get_default_value(const std::string& name);

        public:

                explicit GetOpt(std::vector<Option> &options);
                ~GetOpt() override = default;

                void parse(int argc, char **argv) override;
                bool get_flag(const char *name) override;
                std::string get_value(const std::string& name) override;
                bool is_help_requested() override;
                void print_usage() override;
        };
}

#endif // __ROMI_GETOPT_H

