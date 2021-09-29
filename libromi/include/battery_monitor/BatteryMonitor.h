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
#ifndef __BATTERY_MONITOR__
#define __BATTERY_MONITOR__

#include <RomiSerialClient.h>
#include <RSerial.h>
#include <thread>
#include <atomic>
#include <api/IDataLog.h>
#include <condition_variable>

#include "IBatteryMonitor.h"

namespace romi {


        class BatteryMonitor : public IBatteryMonitor
        {
        public:
            explicit BatteryMonitor(std::unique_ptr<romiserial::IRomiSerialClient>& romi_serial, std::shared_ptr<IDataLog>  dataLog, std::atomic<bool>& application_quit);
            ~BatteryMonitor() override;
            bool enable() override;
            bool disable() override;

        private:
            void monitor_loop();
            bool check_response(const char *command, JsonCpp& response);

            template<class ChronoDuration>
            bool wait_for(ChronoDuration duration) {

                std::unique_lock<std::mutex> l(mutex_);
                return !wait_condition_.wait_for(l, duration, [this]() { return (enabled_==false); });
            }

        private:
            std::unique_ptr<romiserial::IRomiSerialClient> romi_serial_;
            const std::shared_ptr<IDataLog> datalog_;
            std::atomic<bool>& application_quit_;
            std::atomic<bool> enabled_;
            std::unique_ptr<std::thread> monitor_thread_;
            std::condition_variable wait_condition_;
            std::mutex mutex_;
        };
}

#endif // __IBATTERY_MONITOR__
