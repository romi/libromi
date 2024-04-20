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
#ifndef __ROMI_CABLEBOTRUNNER_H
#define __ROMI_CABLEBOTRUNNER_H

#include <memory>
#include "util/IAlarmClockListener.h"
#include "cablebot/ICablebotProgramList.h"
#include "hal/ImagingDevice.h"
#include "session/Session.h"
#include "camera/ICameraInfoIO.h"

namespace romi {

        class CablebotRunner : public IAlarmClockListener
        {
        protected:
                std::shared_ptr<ICablebotProgramList> programs_;
                ImagingDevice& cablebot_;
                Session& session_;
                std::shared_ptr<ICameraInfoIO> info_io_;
                
                void try_run(uint8_t hour, uint8_t minute);
                void run(ICablebotProgram& program);
                void init(ICablebotProgram& program);
                void scan(ICablebotProgram& program);
                void finalize();
                void try_finalize();
                bool moveto(double position, double angle);
                bool grab_image(size_t counter);
                bool store_image(rcom::MemBuffer& image, size_t counter);
                std::string make_image_name(size_t counter);
                
        public:
                CablebotRunner(std::shared_ptr<ICablebotProgramList>& programs,
                               ImagingDevice& cablebot,
                               Session& session,
                               std::shared_ptr<romi::ICameraInfoIO>& info_io);
                ~CablebotRunner() override = default;

                void wakeup(uint8_t hour, uint8_t minute) override;
        };
        
}

#endif // __ROMI_CABLEBOTRUNNER_H

