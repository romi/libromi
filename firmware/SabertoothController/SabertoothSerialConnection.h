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

#ifndef __ROMI_SABERTOOTH_SERIAL_CONNECTION_H
#define __ROMI_SABERTOOTH_SERIAL_CONNECTION_H

#include <SoftwareSerial.h>
#include "IDriverConnection.h"

class SabertoothSerialConnection : public IDriverConnection
{
protected:
        const unsigned char kControllerAddress = 128;
        const unsigned char kLeftDriveForward = 0;
        const unsigned char kLeftDriveBackward = 1;
        const unsigned char kRightDriveForward = 4;
        const unsigned char kRightDriveBackward = 5;

        SoftwareSerial serial_;

public:
        SabertoothSerialConnection(int rxPin, int txPin);
        virtual ~SabertoothSerialConnection() = default;
        
        void setOutputSignal(float l, float r) override;
        void setMaxSignal(float l) override;
        void init() override;

protected:

        
        unsigned char convertSpeed(float speed) {
                if (speed >= 0.0)
                        return (unsigned char) (speed * 127.0);
                else
                        return (unsigned char) (-speed * 127.0);
        }

        void leftDriveAt(float speed) {
                if (speed >= 0.0)
                        leftDriveForwardAt(speed);
                else 
                        leftDriveBackwardAt(speed);
        }

        void leftDriveForwardAt(float speed) {
                sendCommand(kLeftDriveForward, convertSpeed(speed));
        }

        void leftDriveBackwardAt(float speed) {
                sendCommand(kLeftDriveBackward, convertSpeed(speed));
        }

        void rightDriveAt(float speed) {
                if (speed >= 0.0)
                        rightDriveForwardAt(speed);
                else 
                        rightDriveBackwardAt(speed);
        }

        void rightDriveForwardAt(float speed) {
                sendCommand(kRightDriveForward, convertSpeed(speed));
        }

        void rightDriveBackwardAt(float speed) {
                sendCommand(kRightDriveBackward, convertSpeed(speed));
        }

        void sendCommand(unsigned char command, unsigned char data) {
                unsigned char checksum = (kControllerAddress + command + data) & 0b01111111;
                sendByte(kControllerAddress);
                sendByte(command);
                sendByte(data);
                sendByte(checksum);
        }

        void sendByte(unsigned char c) {
                serial_.write(c);
        }
};

#endif // __ROMI_SABERTOOTH_SERIAL_CONNECTION_H
