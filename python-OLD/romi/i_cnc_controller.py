from abc import ABC, abstractmethod
from typing import List

class ICNCController(ABC):
    
    @abstractmethod    
    def enable(self) -> None:
        pass
    
    @abstractmethod    
    def disable(self) -> None:
        pass
    
    @abstractmethod    
    def set_relay(self, value) -> None:
        pass
            
    @abstractmethod    
    def homing(self) -> None:
        pass
            
    @abstractmethod    
    def configure_homing(self, axes) -> None:
        pass
    
    @abstractmethod    
    def wait(self) -> None:
        pass
    
    @abstractmethod    
    def moveat(self, x, y, z) -> None:
        pass
    
    @abstractmethod    
    def move(self, dt, steps_x, steps_y, steps_z) -> None:
        pass
    
    @abstractmethod    
    def get_position(self) -> List[float]:
        pass
