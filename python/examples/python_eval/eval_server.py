import sys
import asyncio
import json

sys.path.append("../..")
from romi.rpc import Server


def handle_eval_request(params):
    expression = params["expression"]
    return eval(expression)


server = Server("python",
                { "eval": handle_eval_request },
                "192.168.43.201")

async def server_callback(websocket, path):
    await server.handle_client(websocket)

async def init():    
    loop = asyncio.get_event_loop()
    registration = loop.create_task(server.register())
    await registration
    await server.start(server_callback)
    
loop = asyncio.get_event_loop()
loop.run_until_complete(init())
asyncio.get_event_loop().run_forever()
