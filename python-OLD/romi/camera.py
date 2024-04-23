import time
import json
import websocket
from PIL import Image
from io import BytesIO
from romi.remote_device import RemoteDevice

class Camera(RemoteDevice):
   
    def __init__(self, topic, registry="127.0.0.1"):
        super().__init__(topic, registry)
    
    def print_error(self, data):
       print(f"Failed to grab the image: {r['error']['message']}")
       
    def grab(self):
        cmd = '{"method": "camera:grab-jpeg-binary"}'
        self.connection.send(cmd, websocket.ABNF.OPCODE_BINARY)
        data = self.connection.recv()
        if type(data) is str:
            if data == "null":
                print("Failed to grab the image")
                return None
            else:
                self.print_error(data)
                return None
        else:
            return Image.open(BytesIO(data))

    def set_value(self, name, value):
        params = {'name': name, 'value': value}
        self.execute('camera:set-value', params)

    def select_option(self, name, value):
        params = {'name': name, 'value': value}
        self.execute('camera:select-option', params)

        
if __name__ == '__main__':
    camera = Camera("top-camera", "10.10.10.1")
    for i in range(10):
        image = camera.grab()
        if image != None:
            print(f"Saving top-camera-{i:05d}.jpg")
            image.save(f"top-camera-{i:05d}.jpg")
