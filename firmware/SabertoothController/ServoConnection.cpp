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

#include <Arduino.h>
#include "ServoConnection.h"

ServoConnection::ServoConnection(int pinLeftMotor, int pinRightMotor, float maxSignal)
        : leftMotor_(), 
          rightMotor_(),
          pinLeftMotor_(pinLeftMotor), 
          pinRightMotor_(pinRightMotor),
          maxSignal_(maxSignal)
{
}

void ServoConnection::init()
{
        leftMotor_.attach(pinLeftMotor_);
        rightMotor_.attach(pinRightMotor_);
        setOutputSignal(0, 0);
}
        
void ServoConnection::setOutputSignal(float l, float r)
{
        leftMotor_.write(convertSpeed(l));
        rightMotor_.write(convertSpeed(r));
}

void ServoConnection::setMaxSignal(float maxSignal)
{
        maxSignal_ = maxSignal;
}
