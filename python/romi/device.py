import serial
import json
import math
import time

        
class RomiDevice():

    def __init__(self, device): 
        print(f"Opening a serial link to {device}")
        self.driver = serial.Serial(device, 115200)
        time.sleep(2.0)
        self.debug = True
        
    def print_debug(self, s):
        if self.debug:
            print(s)
        
    def set_debug(self, value):
        self.debug = value
        
    def get_debug(self):
        return self.debug
        
    def send_command(self, s):
        for i in range(5):
            values = self.try_command(s)
            status_code = values[0]
            if status_code == 0:
                return values
            if status_code > 0:
                raise RuntimeError(values[0])
            if status_code < 0:
                print("Warning: Sending failed. Retrying.")
        raise RuntimeError("Sending failed")
            
    def try_command(self, s):
        command = "#" + s + ":xxxx\r\n"
        self.print_debug(f"Command: {s} -> {command}")
        self.driver.write(command.encode('ascii'))
        reply = self.read_reply()
        self.print_debug(f"Response: {reply}")
        return self.parse_reply(reply)
            
        
    def read_reply(self):
        while True:
            b = self.driver.readline()
            s = b.decode("ascii").rstrip()
            if s[0] == "#":
                if s[1] == "!":
                    print("Log: %s" % s)
                else:
                    break;
        return s
        
    def parse_reply(self, line):
        s = str(line)
        start = s.find("[")
        end = 1 + s.find("]")
        array_str = s[start:end]
        return_values = json.loads(array_str)
        return return_values

    
class MotorController(RomiDevice):

    def __init__(self, device, config): 
        super(MotorController, self).__init__(device)
        self.set_configuration(config)

    def set_configuration(self, config):
        self.config = config
        self.disable()
        self.__send_configuration(config)
        self.enable()
        
    def __send_configuration(self, config):
        steps = config["rover"]["encoder_steps"]
        wheel_diameter = config["rover"]["wheel_diameter"]
        max_speed = config["rover"]["maximum_speed"]
        max_signal = config["brush-motor-driver"]["maximum_signal_amplitude"]
        use_pid = config["brush-motor-driver"]["use_pid"]
        print(f"PID={use_pid}")
        kp = config["brush-motor-driver"]["pid"]["kp"]
        ki = config["brush-motor-driver"]["pid"]["ki"]
        kd = config["brush-motor-driver"]["pid"]["kd"]
        left_encoder = config["brush-motor-driver"]["encoder_directions"]["left"]
        right_encoder = config["brush-motor-driver"]["encoder_directions"]["right"]
        circumference = math.pi * wheel_diameter
        max_rps = max_speed / circumference;
        self.send_command("C[{0},{1},{2},{3},{4},{5},{6},{7},{8}]"
                          .format(int(steps), int(100 * max_rps), int(max_signal),
                                  int(use_pid), int(kp * 1000), int(ki * 1000),
                                  int(kd * 1000), int(left_encoder), int(right_encoder)))
        
    def get_max_rps(self):
        max_speed = self.config["rover"]["maximum_speed"]
        wheel_diameter = self.config["rover"]["wheel_diameter"]
        circumference = wheel_diameter * math.pi
        return max_speed / circumference
        
    def get_encoder_steps(self):
        return self.config["rover"]["encoder_steps"]

    def get_config(self):
        return self.config
    
    def enable(self):
        self.send_command("E[1]")
    
    def disable(self):
        self.send_command("E[0]")
    
    def moveat(self, left, right):
        self.send_command("V[%d,%d]" % (left, right))

    def get_encoder_values(self):
        reply = self.send_command("e")
        # The first element is the status code
        reply.pop(0)
        return reply

    def get_status(self):
        return self.send_command("S")


class OquamController(RomiDevice):

    def __init__(self, device): 
        super(OquamController, self).__init__(device)
    
    def enable(self):
        self.send_command("E[1]")
    
    def disable(self):
        self.send_command("E[0]")
    
    def set_relay(self, value):
        if value == True:
            self.send_command("S[1]")
        elif value == False:
            self.send_command("S[0]")
        else:
            raise TypeError("Expected boolean")
            
    def homing(self):
        self.send_command("H")
            
    def configure_homing(self, axes):
        self.send_command("h[%d,%d,%d]" % (axes[0], axes[1], axes[2]))
    
    def wait(self):
        while True:
            status = self.send_command("I")
            if status[1] == 1:
                break
            time.sleep(1);
    
    def moveat(self, x, y, z):
        self.send_command("V[%d,%d,%d]" % (x, y, z))
    
    def move(self, dt, steps_x, steps_y, steps_z):
        self.send_command("M[%d,%d,%d,%d]"
                          % (dt, steps_x, steps_y, steps_z))
