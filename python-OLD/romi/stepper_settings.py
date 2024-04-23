import math
import numpy as np

class StepperSettings():

    def __init__(self, config): 
        self.steps_per_revolution = np.array(config['steps-per-revolution'])
        self.microsteps = np.array(config['microsteps'])
        self.gears_ratio = np.array(config['gears-ratio'])
        self.maximum_rpm = np.array(config['maximum-rpm'])
        self.displacement_per_revolution = np.array(config['displacement-per-revolution'])
        self.maximum_acceleration = np.array(config['maximum-acceleration'])
        self.compute_maximum_speed()
        self.compute_steps_per_unit()
        
    def compute_maximum_speed(self):
        self.maximum_speed = np.abs(self.displacement_per_revolution) * self.maximum_rpm / 60.0 / self.gears_ratio
            
    def compute_steps_per_unit(self):
        self.steps_per_unit  = (self.gears_ratio * self.microsteps * self.steps_per_revolution
                                / self.displacement_per_revolution)

        
