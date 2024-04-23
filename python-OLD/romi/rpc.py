import time
import sys
import asyncio
import websockets
import json
    
class Registry():
    def __init__(self, ip="127.0.0.1"):
        self.addr = f"ws://{ip}:10101"
        print(f"Using registry @ {self.addr}")
    
    async def get_address(self, topic):
        print(f"Requesting address for topic {topic}, registry at {self.addr}")
        reply = await self.send_get_topic(topic)
        if reply["success"]:
            return reply["address"]
        else:
            raise Exception("try_get_address: registry signaled error: %s"
                            % reply["message"])
    
    async def send_get_topic(self, topic):
        return await self.send_request({ "request": "get", "topic": topic })

    async def register(self, topic, address, timeout):
        print(f"Registering address {address} for topic {topic}, registry at {self.addr}")
        reply = await self.send_register(topic, address)
        if reply["success"]:
            print("Registered successfully")
            return True
        else:
            print("try_register: failed: %s" % reply["message"])
            return False
    
    async def send_register(self, topic, address):
        return await self.send_request({ "request": "register",
                                   "topic": topic,
                                   "address": address })

    async def send_request(self, request):
        print(f"send_request {self.addr}")
        async with websockets.connect(self.addr) as ws:
            print(f"Sending request to {self.addr}: %s" % json.dumps(request))
            await ws.send(json.dumps(request))
            response = await ws.recv()
            return json.loads(response)
    

class Server:
    def __init__(self, topic, handlers, registry="127.0.0.1",
                 ip="127.0.0.1", port=10111):
        self.topic = topic
        self.handlers = handlers
        self.registry = registry
        self.ip = ip
        self.port = port
        
    async def register(self):
        registry = Registry(self.registry)
        await registry.register(self.topic, f"{self.ip}:{self.port}", 10)

    def start(self, callback):
        print("======================================================== ping_interval=None")
        self.server = websockets.serve(callback, self.ip, self.port, ping_interval=None)
        return self.server
    
    async def handle_client(self, websocket):
        try:
            while True:
                data = await websocket.recv()
                response = self.handle_request(data)
                print(json.dumps(response))
                await websocket.send(json.dumps(response))
        except websockets.exceptions.ConnectionClosedOK:
            pass
    
    def handle_request(self, data):
        try:
            request = json.loads(data)
            method = request['method']
            params = request['params']
            result = self.handle_method(method, params)
            print("results: %s" % str(result))
            return {'error': { 'code': 0 }, 'result': result}
        except Exception as e:
            return {'error': { 'code': 1, 'message': str(e) }}

    def handle_method(self, method, params):
        handler = self.handlers
        if method in self.handlers:
            handler = self.handlers[method]
            return handler(params)
        else:
            raise ValueError(f"Unknown method: {method}")
    
class Client:
    def __init__(self, topic, registry="127.0.0.1"):
        self.topic = topic
        self.registry = registry

    async def connect(self):
        registry = Registry(self.registry)
        addr = await registry.get_address(self.topic)
        self.addr = "ws://" + addr
        self.ws = await websockets.connect(self.addr, ping_interval=None)

    async def execute(self, method, **kwargs):
        request = { 'method': method, 'params': kwargs }
        print(f"Sending request to {self.addr}: %s" % json.dumps(request))
        response = await self.__rpc(json.dumps(request))
        print(f"Got response: %s" % json.dumps(response))
        return json.loads(response)

    async def __rpc(self, data):
        await self.ws.send(data)
        return await self.ws.recv()

    
async def create_client(topic, ip="127.0.0.1"):
    client = Client(topic, ip)
    await client.connect()
    return client

