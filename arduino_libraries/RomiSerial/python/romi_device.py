"""
  romi_device.py

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  romi_device.py is part of the romi-rover project, a collection of 
  applications for the Romi Rover and other ROMI devices.

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
"""
import time
import serial
import json
import crc8
import string

class EnvelopeEncoder():
    def __init__(self): 
        self.counter = 0
        self.opcodes = string.ascii_lowercase + string.ascii_uppercase + string.digits + '?'
        
    def convert_string(self, s):
        #return self._create_command_simple(s)
        return self._create_command_with_crc(s)

    def convert(self, opcode, *args):
        self._assert_opcode(opcode)
        self._assert_args(*args)
        arguments = ''
        if len(args) > 0:
            arguments = ','.join(self._str(x) for x in args)
            arguments = f'[{arguments}]'
        command = f'{opcode}{arguments}'
        return self.convert_string(command)

    def _assert_opcode(self, opcode):
        if len(opcode) != 1:
            raise ValueError(f'Opcode can have only one character: {opcode}')
        if self.opcodes.find(opcode) == -1:
            raise ValueError(f'Invalid opcode: {opcode}')
    
    def _assert_args(self, *args):
        if len(args) > 12:
            raise ValueError(f'Too many arguments: {len(args)} > 12')
        if self._count_strings(*args) > 1:
            raise ValueError(f'Too many strings')
        self._assert_types(*args)
    
    def _count_strings(self, *args):
        count = 0
        for x in args:
            if type(x) == str:
                count = count + 1
        return count
    
    def _assert_types(self, *args):
        for x in args:
            self._assert_type(x)

    def _assert_type(self, arg):
        if type(arg) != str and type(arg) != int:
            raise ValueError(f'Unsupported agrument type: {type(arg)}')
    
    def _str(self, arg):
        if type(arg) == str:
            return f'"{arg}"'
        else:
            return str(arg)

    def _assert_string_length(self, s):
        if len(s) > 58:
            raise ValueError(f'The command it too long: {len(s)} > 58')
        
    def _create_command_simple(self, s):
        self._assert_string_length(s)
        command = "#" + s + ":xxxx\r\n"
        return command
    
    def _create_command_with_crc(self, s):
        self._assert_string_length(s)
        partial_command = f'#{s}:{self.counter:02x}'
        crc = self._compute_crc(partial_command)
        command = f'{partial_command}{crc}\r\n'
        self._increment_counter()
        return command

    def _compute_crc(self, s):
        h = crc8.crc8()
        h.update(s.encode('utf-8'))
        return h.hexdigest()

    def _increment_counter(self):
        self.counter = (self.counter + 1) % 255
    

class EnvelopeDecoder():
    
    def parse(self, line):
        s = str(line)
        start = s.find("[")
        end = 1 + s.find("]")
        array_str = s[start:end]
        return_values = json.loads(array_str)
        return return_values

    def is_valid_message(self, line):
        return (len(line) > 1
                and line[0] == "#")

    def is_log_message(self, line):
        return (len(line) > 2
                and line[0] == "#"
                and line[1] == "!")
    

class RomiDevice():

    def __init__(self, device): 
        print(f"Opening a serial link to {device}")
        self.driver = serial.Serial(device, 115200)
        time.sleep(2.0)
        self.encoder = EnvelopeEncoder()
        self.decoder = EnvelopeDecoder()
        self.debug = False
        
    def get_driver(self):
        return self.driver
        
    def print_debug(self, s):
        if self.debug:
            print(s)
        
    def set_debug(self, value):
        self.debug = value
        
    def get_debug(self):
        return self.debug
        
    def execute(self, opcode, *args):
        command = self.encoder.convert(opcode, *args)
        self.print_debug(f"Command: {opcode}, {args} -> {command}")
        return self._iterate_command(command, 5)
        
    def send_command(self, s):
        command = self.encoder.convert_string(s)
        self.print_debug(f"Command: {s} -> {command}")
        return self._iterate_command(command, 5)

    def _iterate_command(self, command, n):
        for i in range(n):
            values = self._try_send_command(command)
            status_code = values[0]
            if status_code == 0:
                return values
            if status_code > 0:
                raise RuntimeError(values[0])
            if status_code < 0:
                print("Warning: Sending failed. Retrying.")
        raise RuntimeError("Sending failed")
    
    def _try_send_command(self, command):
        self.driver.write(command.encode('ascii'))
        reply = self.read_reply()
        self.print_debug(f"Response: {reply}")
        return self.decoder.parse(reply)
            
    def read_reply(self):
        done = False
        s = ''
        while not done:
            b = self.driver.readline()
            s = b.decode("ascii").rstrip()
            self.print_debug(f"s='{s}'")
            if self.decoder.is_valid_message(s):
                if self.decoder.is_log_message(s):
                    self._print_log(s)
                else:
                    done = True
        return s

    def _print_log(self, line):
        print(f'Log: {line}')



if __name__ == '__main__':

    test = EnvelopeEncoder()

    #
    user_command = 'e[0]'
    coded_command = test.convert_string(user_command)
    expected_command = '#e[0]:0092\r\n'
    if coded_command != expected_command:
        print('Test 1: Failed: {coded_command} != {expected_command}')
    else:
        print('Test 1: OK')

    #
    coded_command = test.convert('e', 0, 1, 'dummi')
    expected_command = '#e[0,1,"dummi"]:0140\r\n'
    if coded_command != expected_command:
        print('Test 2: Failed: {coded_command} != {expected_command}')
    else:
        print('Test 2: OK')

    #
    try:
        test.convert('abba', 0, 1, 'dummi')
        print('Test 3: Failed: expected exception')
    except (RuntimeError, ValueError) as e:
        print({e})
        print('Test 3: OK')

    #
    try:
        test.convert('%', 0, 1, 'dummi')
        print('Test 4: Failed: expected exception')
    except (RuntimeError, ValueError) as e:
        print({e})
        print('Test 4: OK')

    #
    try:
        test.convert('e', 0, 1.2)
        print('Test 5: Failed: expected exception')
    except (RuntimeError, ValueError) as e:
        print({e})
        print('Test 5: OK')

    #
    try:
        test.convert('e', 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)
        print('Test 6: Failed: expected exception')
    except (RuntimeError, ValueError) as e:
        print({e})
        print('Test 6: OK')

    #
    try:
        test.convert('e', 0, 1, 'dummi', 'dummi-too')
        print('Test 7: Failed: expected exception')
    except (RuntimeError, ValueError) as e:
        print({e})
        print('Test 7: OK')

    #
    try:
        test.convert('e', '012345678901234567890123456789012345678901234567890123456789')
        print('Test 8: Failed: expected exception')
    except (RuntimeError, ValueError) as e:
        print({e})
        print('Test 8: OK')
