#include "battery_monitor/BatteryMonitor.h"
#include <atomic>
#include <utility>
#include <r.h>
#include <iostream>

namespace romi
{
    static const std::vector<std::string> data_names {"battery-voltage", "battery-aux-voltage", "battery-current" , "battery-instant-power", "battery-consumed-a-h","battery-state-of-charge", "battery-time-to-go", "battery-alarm-active", "battery-alarm-reason"};

    BatteryMonitor::BatteryMonitor(std::unique_ptr<romiserial::IRomiSerialClient>& romi_serial, std::shared_ptr<IDataLog>  dataLog, std::atomic<bool>& quit)
            :
            romi_serial_(std::move(romi_serial)),
            datalog_(std::move(dataLog)),
            application_quit_(quit),
            enabled_(false),
            monitor_thread_(),
            wait_condition_(),
            mutex_()
    {
        if (romi_serial_ == nullptr)
            throw std::runtime_error("BatteryMonitor::BatteryMonitor() romi_serial == nullptr");
        if (datalog_ == nullptr)
            throw std::runtime_error("BatteryMonitor::BatteryMonitor() dataLog_ == nullptr");
    }

    bool romi::BatteryMonitor::enable() {
        if (!enabled_) {
            enabled_ = true;
            monitor_thread_ = std::make_unique<std::thread>([this] { monitor_loop(); });
        }
        return enabled_;
    }

    bool romi::BatteryMonitor::disable() {
        enabled_ = false;
        monitor_thread_->join();
        return true;
    }

    bool BatteryMonitor::check_response(const char *command, JsonCpp& response)
    {
        bool success = (response.num(romiserial::kStatusCode) == 0);
        if (!success) {
            const char *message = "";
            if (response.length() > 1)
                message = response.str(romiserial::kErrorMessage);
            r_warn("BatteryMonitor: command %s returned error: %s",
                   command, message);
        }
        return success;
    }

    void BatteryMonitor::monitor_loop() {
        try {
            using namespace std::chrono_literals;
            while((!application_quit_) && (enabled_))
            {
                JsonCpp response;
                std::string command("B");
                romi_serial_->send(command.c_str(), response);
                if (check_response(command.c_str(), response))
                {
                    // Index 0 of the response is the success/ failure of the send function. Loop from 1;
                    for (size_t data_index = 1; data_index < response.length(); data_index++)
                    {
                        auto element = response.get(data_index);
                        if (element.isnumber())
                        {
                            double value = response.num(data_index);
                            datalog_->store(data_names[data_index-1], value);
                        }
                    }
                }
                wait_for(2s);
            }

        }
        catch(std::exception& e)
        {
            r_err("BatteryMonitor::monitor_loop %. quitting.", e.what());
            enabled_ = false;
        }
    }

    BatteryMonitor::~BatteryMonitor() {
        enabled_ = false;
        monitor_thread_->join();
    }
}

