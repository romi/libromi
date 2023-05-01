import sys
sys.path.append('../python')

import time
import argparse
from romi_device import RomiDevice

remoteDevice = None

def setup(device):
    global remoteDevice
    remoteDevice = RomiDevice(device)

    
def loop():
    global remoteDevice
    response = remoteDevice.execute('A')
    print(f'Sensor value: {response[1]}')
    time.sleep(1)
    

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--device', type=str, nargs='?', default="COM5",
                    help='The serial device to connect to')
    args = parser.parse_args()
    
    setup(args.device)
    while True:
        loop()
    
