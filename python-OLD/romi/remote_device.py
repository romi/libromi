import time
import json
import websocket
import argparse
import math
from PIL import Image
from io import BytesIO

class RemoteDevice():

    def __init__(self, topic, registry='127.0.0.1'):
        registry = websocket.create_connection(f'ws://{registry}:10101')
        cmd = '{"request": "get", "topic": "%s"}' % topic
        registry.send(cmd)
        data = registry.recv()
        response = json.loads(data)
        print(response)
        print(f"Connecting to ws://{response['address']}")
        self.connection = websocket.create_connection(f"ws://{response['address']}")

    def check_error(self, response):
        if 'error' in response:
            error = response['error']
            print(f"Request failed: {error['message']}")
            raise RuntimeError(error['message'])
            
    def execute(self, method, params=None):
        self.send_request(method, params)
        return self.read_response()
    
    def send_request(self, method, params):
        if params != None:
            cmd = { 'method': method, 'params': params }
        else:
            cmd = { 'method': method }
        request = json.dumps(cmd)
        print(f"Request: {request}")
        self.connection.send(request)
        
    def read_response(self):
        response = json.loads(self.connection.recv())
        print(f"Response: {response}")
        self.check_error(response)
        if 'result' in response:
            result = response['result']
        else: result = None
        return result
    
    

class Oquam(RemoteDevice):
   
    def __init__(self, topic, registry="127.0.0.1"):
        super().__init__(topic, registry)
       
    def homing(self):
        self.execute('cnc-homing')
       
    def power_up(self):
        self.execute('power-up')
        
    def power_down(self):
        self.execute('power-down')
       
    def get_position(self):
        return self.execute('cnc-get-position')
       
    def get_range(self):
        return self.execute('cnc-get-range')
       
    def moveto(self, x, y, z, speed):
        params = {}
        if x != None:
            params["x"] = x
        if y != None:
            params["y"] = y
        if z != None:
            params["z"] = z
        if speed != None:
            params["speed"] = speed
        self.execute('cnc-moveto', params)
       
    def helix(self, xc, yc, alpha, z, speed):
        params = { "xc": xc, "yc": yc, "z": z, "alpha": alpha, "speed": speed }
        self.execute('cnc-helix', params)

        
class OquamXYTheta(Oquam):
   
    def __init__(self, topic, registry="127.0.0.1"):
        super().__init__(topic, registry)

    def current_theta(self):
        position = self.get_position()
        return position['z']

    def get_absolute_theta(self, theta):
        return theta + self.current_theta()
    
    def moveto(self, x, y, theta, speed, absolute=False):
        if not absolute:
            theta = self.get_absolute_theta(theta)
        super().moveto(x, y, theta, speed)
    
    def helix(self, xc, yc, alpha, theta, speed, absolute=False):
        if not absolute:
            theta = self.get_absolute_theta(theta)
        super().helix(xc, yc, alpha, theta, speed)

        
class Camera(RemoteDevice):
   
    def __init__(self, topic, registry="127.0.0.1"):
        super().__init__(topic, registry)
    
    def print_error(self, data):
       r = json.loads(data)
       print(f"Failed to grab the image: {r['error']['message']}")
       
    def grab(self):
        cmd = '{"method": "camera-grab-jpeg-binary"}'
        self.connection.send(cmd, websocket.ABNF.OPCODE_BINARY)
        data = self.connection.recv()
        if type(data) is str:
            self.print_error(data)
            return None
        else:
            return Image.open(BytesIO(data))

        
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--registry', type=str, nargs='?', default="127.0.0.1",
                    help='IP address of the registry')
    args = parser.parse_args()
    
    cnc = OquamXYTheta("cnc", args.registry)
    cnc.power_up()

    dimension = cnc.get_range()
    print(f"dimension={dimension}")

    xc = (dimension[0][0] + dimension[0][1]) / 2.0
    yc = (dimension[1][0] + dimension[1][1]) / 2.0
    d = min(dimension[0][1] - dimension[0][0],
            dimension[1][1] - dimension[1][0])
    r = (d - 0.02) / 2
    print(f"c=({xc}, {yc}), r={r}")
    
    cnc.moveto(xc - r, yc, 0.0, 0.5)
    
    cnc.helix(xc, yc, -2.0 * math.pi, -2.0 * math.pi, 1.0)

    position = cnc.get_position()
    print(f"position={position}")
    
    n = 32
    delta_alpha = -2.0 * math.pi / n
    start_time = time.time()
    for i in range(n):
        cnc.helix(xc, yc, delta_alpha, delta_alpha, 1.0)
    print(f"duration: {time.time()-start_time}")
