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
#include <thread>
#include <ClockAccessor.h>
#include <r.h>
#include "api/DataLog.h"

namespace romi {

        using SynchronizedCodeBlock = std::lock_guard<std::mutex>;
        
        DataLog::DataLog(const std::string& path)
                : name_to_index_(),
                  index_to_name_(),
                  entries_(),
                  mutex_vector_(),
                  mutex_map_(),
                  fp_(nullptr),
                  thread_(nullptr),
                  quitting_(false),
                  hub_(),
                  message_(),
                  last_handle_events_(0.0)
        {
                fp_ = fopen(path.c_str(), "w");
                if (fp_ == nullptr) {
                        r_err("DataLog: can't open file %s", path.c_str());
                        throw std::runtime_error("DataLog: can't open file");
                }
                try_create_hub();
                thread_ = std::make_unique<std::thread>([this]() {
                                this->write_entries_to_storage_in_background();
                        });
        }
        
        DataLog::DataLog(const std::filesystem::path& filepath)
                : DataLog(filepath.string())
        {
        }
        
        DataLog::~DataLog()
        {
                quitting_ = true;
                if (thread_ != nullptr)
                        thread_->join();
                fclose(fp_);
        }

        void DataLog::try_create_hub()
        {
                try {
                        hub_ = std::make_unique<rcom::MessageHub>("datalog");

                } catch (const std::runtime_error& re) {
                        r_err("Failed to create the message hub: %s", re.what());
                        hub_ = nullptr;
                }
        }
        
        void DataLog::store(const std::string& name, double value)
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                store(clock->time(), name, value);
        }
        
        void DataLog::store(double time, const std::string& name, double value)
        {
                SynchronizedCodeBlock synchronize(mutex_vector_);
                store_in_queue(time, name, value);
                handle_events(time);
        }
        
        void DataLog::store_in_queue(double time, const std::string& name, double value)
        {
                uint32_t index = get_index(name);
                entries_.emplace_back(time, index, value);
        }
        
        void DataLog::handle_events(double time)
        {
                if (hub_ != nullptr
                    && time - last_handle_events_ >= 1.0) {
                        last_handle_events_ = time;
                        hub_->handle_events();
                }
        }

        uint32_t DataLog::get_index(const std::string& name)
        {
                SynchronizedCodeBlock synchronize(mutex_map_);
                uint32_t index;
                std::map<std::string,uint32_t>::iterator it;
                
                it = name_to_index_.find(name);
                if (it != name_to_index_.end()) {
                        index = it->second;
                } else {
                        index = (uint32_t) name_to_index_.size();
                        name_to_index_.insert(std::pair<std::string,uint32_t>(name, index));
                        index_to_name_.insert(std::pair<uint32_t,std::string>(index, name));
                }
                return index;
        }
        
        void DataLog::write_entries_to_storage_in_background()
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                
                while (!quitting_) {
                        try_writing_entries_to_storage();
                        clock->sleep(1.0);
                }

                // Make sure the remaining entries get stored.
                try_writing_entries_to_storage();
        }
                
        void DataLog::try_writing_entries_to_storage()
        {
                try {
                        std::vector<DataLogEntry> entries;
                        copy_entries(entries);
                        if (entries.size() > 0) {
                                write_entries_to_storage(entries);
                                transmit_entries(entries);
                        }

                } catch (const std::runtime_error& e) {
                        r_err("DataLog: failed to store the data: %s", e.what());
                }
        }
        
        void DataLog::copy_entries(std::vector<DataLogEntry>& entries)
        {
                // The maximum time that store() can get blocked is
                // the time required to make this copy.
                SynchronizedCodeBlock synchronize(mutex_vector_);
                entries = entries_;
                entries_.clear();
        }
        
        void DataLog::write_entries_to_storage(std::vector<DataLogEntry>& entries)
        {
                for (auto entry: entries)
                        write_entry_to_storage(entry);
                fflush(fp_);
        }
        
        void DataLog::write_entry_to_storage(DataLogEntry& entry)
        {
                const std::string& name = get_name(entry.index_);
                fprintf(fp_, "%f,%s,%f\n", entry.time_, name.c_str(), entry.value_); 
        }
        
        void DataLog::transmit_entries(std::vector<DataLogEntry>& entries)
        {
                if (hub_ != nullptr
                    && hub_->count_links() > 0) {
                        message_.clear();
                        append_entries(entries);
                        hub_->broadcast(message_, rcom::kTextMessage, nullptr);
                }
        }
        
        void DataLog::append_entries(std::vector<DataLogEntry>& entries)
        {
                message_.printf("[");
                for (size_t i = 0; i < entries.size() - 1; i++) {
                        append_entry(entries[i]);
                        message_.printf(",");
                }
                append_entry(entries[entries.size() - 1]);
                message_.printf("]");
        }
        
        void DataLog::append_entry(DataLogEntry& entry)
        {
                const std::string& name = get_name(entry.index_);
                message_.printf("[%f,\"%s\",%f]", entry.time_, name.c_str(), entry.value_);
        }

        const std::string& DataLog::get_name(uint32_t index)
        {
                SynchronizedCodeBlock synchronize(mutex_map_);
                std::map<uint32_t,std::string>::iterator it;
                
                it = index_to_name_.find(index);
                if (it == index_to_name_.end()) {
                        r_err("DataLog: Couldn't find name for index %u", index);
                        throw std::runtime_error("DataLog: couldn't find name");
                } 
                return it->second;                
        }
}

