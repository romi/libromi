import sys
sys.path.append("..")

import asyncio
from romi.rpc import Registry

async def get_address():
    registry = Registry("192.168.43.201")
    address = await registry.get_address("toto")
    print(address)
    
loop = asyncio.get_event_loop()
loop.run_until_complete(get_address())
loop.close()
