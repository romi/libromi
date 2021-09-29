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

#ifndef __ROMI_DATALOG_H
#define __ROMI_DATALOG_H

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <filesystem>
#include <MessageHub.h>
#include <IMessageListener.h>
#include "api/IDataLog.h"

namespace romi {

        struct DataLogEntry
        {
                double time_;
                uint32_t index_;
                double value_;

                DataLogEntry(double time, uint32_t index, double value)
                        : time_(time), index_(index), value_(value) {
                }
        };

        class DataLog : public IDataLog
        {
        protected:

                static const size_t kCacheSize = 100;
                
                std::map<std::string, uint32_t> name_to_index_;
                std::map<uint32_t, std::string> index_to_name_;
                std::vector<DataLogEntry> entries_;
                std::mutex mutex_vector_;
                std::mutex mutex_map_;
                FILE *fp_;
                std::unique_ptr<std::thread> thread_;
                std::atomic<bool> quitting_;
                std::unique_ptr<rcom::MessageHub> hub_;
                rpp::MemBuffer message_;
                double last_handle_events_;
                
                uint32_t get_index(const std::string& name);
                void write_entries_to_storage_in_background();
                void try_writing_entries_to_storage();
                void write_entries_to_storage(std::vector<DataLogEntry>& entries);
                void write_entry_to_storage(DataLogEntry& entry);
                const std::string& get_name(uint32_t index);
                void copy_entries(std::vector<DataLogEntry>& entries);
                void store_in_queue(double time, const std::string& name, double value);
                void transmit_entries(std::vector<DataLogEntry>& entries);
                void append_entries(std::vector<DataLogEntry>& entries);
                void append_entry(DataLogEntry& entry);
                void handle_events(double time);
                void try_create_hub();
                
                DataLog(const DataLog& other) = delete;
                DataLog& operator=(const DataLog& other) = delete;
        
        public:
                DataLog(const std::string& filepath);
                DataLog(const std::filesystem::path& filepath);
                ~DataLog() override;
                void store(double time, const std::string& name, double value) override;
                void store(const std::string& name, double value) override;
        };
}

#endif // __ROMI_DATALOG_H
