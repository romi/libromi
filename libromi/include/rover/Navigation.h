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
#ifndef __ROMI_DEFAULT_NAVIGATION_H
#define __ROMI_DEFAULT_NAVIGATION_H

#include <mutex>
#include <thread>
#include <atomic>

#include "api/INavigation.h"
#include "api/IMotorDriver.h"
#include "rover/NavigationSettings.h"
#include "session/ISession.h"
#include "data_provider/ILocationProvider.h"
#include "data_provider/IOrientationProvider.h"
#include "rover/ITrackFollower.h"
#include "rover/INavigationController.h"
#include "rover/WheelOdometry.h"

namespace romi {

        using SynchronizedCodeBlock = std::lock_guard<std::mutex>;

        class Navigation : public INavigation
        {
        protected:

                enum move_status_t{ MOVEAT_CAPABLE, MOVING };
                
                IMotorDriver& driver_;
                NavigationSettings& settings_;
                ITrackFollower& track_follower_;
                ISession& session_;
                std::mutex mutex_;
                move_status_t status_;
                bool stop_;

                // WIP
                std::unique_ptr<std::thread> update_thread_;
                double max_acceleration_; // in m/s²
                std::atomic<double> left_target_;
                std::atomic<double> right_target_;
                double left_speed_;
                double right_speed_;
                std::atomic<bool> quitting_;
                void update_speeds();
                bool set_speed_targets(double left, double right);
                double compute_next_speed(double current_speed, double target_speed,
                                          double dt);
                //
                
                bool send_moveat(double left, double right);
                bool do_move(double distance, double speed);
                bool travel(double speed, double distance);
                bool try_travel(ILocationProvider& location_provider,
                                INavigationController& navigation_controller,
                                double speed, double distance, double timeout);
                double compute_timeout(double distance, double speed);
                                        
        public:
                
                Navigation(IMotorDriver &driver,
                           NavigationSettings &settings,
                           ITrackFollower& track_follower,
                           ISession& session);
                ~Navigation() override;

                bool moveat(double left, double right) override;
                bool move(double distance, double speed) override;
                bool stop() override;
                
                bool pause_activity() override;
                bool continue_activity() override;
                bool reset_activity() override;
        };
}

#endif // __ROMI_DEFAULT_NAVIGATION_H
