/*
  Romi motor controller for brushed mortors

  Copyright (C) 2018-2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  The Romi Brush Controller is free software: you can
  redistribute it and/or modify it under the terms of the GNU Lesser
  General Public License as published by the Free Software Foundation,
  either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */

#ifndef __ROMI_I_DRIVER_CONNECTION_H
#define __ROMI_I_DRIVER_CONNECTION_H

class IDriverConnection
{
public:
        virtual ~IDriverConnection() = default;
        virtual void init() = 0;
        virtual void setOutputSignal(float l, float r) = 0;
        virtual void setMaxSignal(float l) = 0;
};

#endif // __ROMI_I_DRIVER_CONNECTION_H
