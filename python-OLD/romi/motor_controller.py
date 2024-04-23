import serial
import json
import math
from romi.romi_device import RomiDevice

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
        # Encoder
        steps = config["brush-motor-driver"]["encoder-steps"]
        left_encoder = config["brush-motor-driver"]["encoder-directions"]["left"]
        right_encoder = config["brush-motor-driver"]["encoder-directions"]["right"]
        
        # Speed envelope
        wheel_diameter = config["rover"]["wheel-diameter"]
        circumference = math.pi * wheel_diameter
        max_speed = config["rover"]["maximum-speed"]
        max_angular_speed = max_speed / circumference;
        max_acceleration = config["rover"]["maximum-acceleration"]
        max_angular_acceleration = max_acceleration / circumference
        
        # PI controller
        kp_numerator = config["brush-motor-driver"]["pid"]["kp"][0]
        kp_denominator = config["brush-motor-driver"]["pid"]["kp"][1]
        ki_numerator = config["brush-motor-driver"]["pid"]["ki"][0]
        ki_denominator = config["brush-motor-driver"]["pid"]["ki"][1]
        max_signal = config["brush-motor-driver"]["maximum-signal-amplitude"]
        
        self.send_command("C[{0},{1},{2},{3},{4},{5},{6},{7},{8},{9}]"
                          .format(int(steps), int(left_encoder), int(right_encoder),
                                  int(1000 * max_angular_speed),
                                  int(1000 * max_angular_acceleration),
                                  int(kp_numerator), int(kp_denominator),
                                  int(ki_numerator), int(ki_denominator),
                                  int(max_signal)))
        
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
