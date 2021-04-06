/*
  romi-rover

  Copyright (C) 2019-2020 Sony Computer Science Laboratories
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

#include "Log.h"
#include "RomiSerial.h"
#include "RomiSerialErrors.h"
#include <stdio.h>

RomiSerial::RomiSerial(IInputStream& in, IOutputStream& out,
                       const MessageHandler *handlers, uint8_t num_handlers)
        : _in(in), _out(out),
          _handlers(handlers),
          _num_handlers(num_handlers),
          _envelopeParser(),
          _messageParser(),
          _sent_response(false),
          _crc(),
          _last_id(255)
{
}

void RomiSerial::handle_input()
{
        char c;
        while (_in.available()) {
                if (_in.read(c) > 0) {
                        handle_char(c);
                }
        }
}

void RomiSerial::handle_char(char c)
{
        bool has_message = _envelopeParser.process(c);
        if (has_message) {
                process_message();
                
        } else if (_envelopeParser.error() != 0) {
                send_error(_envelopeParser.error(), nullptr);
        }
}

void RomiSerial::process_message()
{
        if (_envelopeParser.has_id()) {
                if (_envelopeParser.id() == _last_id) {
                        // This message was already handled.
                        send_error(romiserial_duplicate, nullptr);
                } else {
                        parse_and_handle_message();
                }
        } else {
                // The incoming message has no ID. This happens for
                // messages sent on the command line that terminate
                // with :xxxx
                parse_and_handle_message();
        }
}

void RomiSerial::parse_and_handle_message()
{
        bool ok = _messageParser.parse(_envelopeParser.message(),
                                       _envelopeParser.length());
        if (ok) {
                handle_message();

        } else {
                send_error(_messageParser.error(), nullptr);
        }
}

void RomiSerial::handle_message()
{
        int index = get_handler();

        if (index < 0) {
                send_error(romiserial_unknown_opcode, nullptr);

        } else if (assert_valid_arguments(index)) {
                _sent_response = false;
                _handlers[index].callback(this,
                                          _messageParser.values(),
                                          _messageParser.string());
                if (!_sent_response) {
                        send_error(romiserial_bad_handler, nullptr);
                }
        }
}

int RomiSerial::get_handler()
{
        int index = -1;
        for (int i = 0; i < _num_handlers; i++) {
                if (_messageParser.opcode() == _handlers[i].opcode) {
                        index = i;
                        break;
                }
        }
        return index;
}

bool RomiSerial::assert_valid_arguments(int index)
{
        return (assert_valid_argument_count(index)
                && assert_valid_string_argument(index));
}

bool RomiSerial::assert_valid_argument_count(int index)
{
        bool retval = false;
        if (_messageParser.length() == _handlers[index].number_arguments) {
                retval = true;
        } else {
                send_error(romiserial_bad_number_of_arguments, nullptr);
        }
        return retval;
}

bool RomiSerial::assert_valid_string_argument(int index)
{
        bool retval = false;
        if (_messageParser.has_string() == _handlers[index].requires_string) {
                retval = true;
        } else {
                if (_handlers[index].requires_string)
                        send_error(romiserial_missing_string, nullptr);
                else
                        send_error(romiserial_bad_string, nullptr);
        }
        return retval;
}

void RomiSerial::append_char(const char c)
{        
        _crc.update(c);
        _out.write(c);
}

void RomiSerial::append_message(const char *s)
{
        while (*s != '\0')
                append_char(*s++);
}

void RomiSerial::start_message()
{
        _crc.start();
        append_char('#');
        if (_messageParser.opcode()) 
                append_char(_messageParser.opcode());
        else
                append_char('_');
}

void RomiSerial::finalize_message()
{
        append_char(':');
        append_id();
        append_crc();
        append_char('\r');
        append_char('\n');
}

void RomiSerial::send_error(int code, const char *message)
{
        char buffer[128];
        if (message)
                snprintf(buffer, sizeof(buffer), "[%d,\"%s\"]", code, message);
        else
                snprintf(buffer, sizeof(buffer), "[%d]", code);
        send_message(buffer);
        _sent_response = true;
}

void RomiSerial::send_ok()
{
        send_message("[0]");
        _sent_response = true;
        _last_id = _envelopeParser.id();
}

void RomiSerial::send(const char *message)
{
        send_message(message);
        _sent_response = true;
        _last_id = _envelopeParser.id();
}

void RomiSerial::send_message(const char *message)
{
        start_message();
        append_message(message);
        finalize_message();
}

// TBD: This code is duplicated in 3 places.
// test_romiserial.cpp, RomiSerial.h, RomiSeralClient.cpp
char RomiSerial::convert_4bits_to_hex(uint8_t value)
{
        value &= 0x0f;
        return (value < 10)? (char)('0' + value) : (char)('a' + (value - 10));
}

void RomiSerial::append_hex(uint8_t value)
{
        append_char(convert_4bits_to_hex((uint8_t)(value >> 4)));
        append_char(convert_4bits_to_hex(value));
}

void RomiSerial::append_id()
{
        append_hex(_envelopeParser.id());
}

void RomiSerial::append_crc()
{
        append_hex(_crc.get());
}

void RomiSerial::log(const char *message)
{
        _out.print("#!");
        _out.print(message);
        _out.print(":xxxx\r\n");
}
