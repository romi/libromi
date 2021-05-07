import sys
import asyncio
import numpy as np
from sklearn.decomposition import PCA
import cv2
import json

sys.path.append("..")
from romi.rpc import Server


def pca(infile):
    print(f"PCA: {infile}")
    im = cv2.imread(infile, 0)
    X = np.squeeze(np.where(im==255)).T #get coordinates of white pixels
    pca = PCA(n_components=2)
    pca.fit(X)
    center = pca.mean_
    components = pca.components_

    # generate an image showing the components, for debugging
    if True: 
        v = pca.explained_variance_
        pc0 = center + components[0] * np.sqrt(v[0])
        pc1 = center + components[1] * np.sqrt(v[1])
        res = cv2.imread(infile)
        cv2.line(res,
                 tuple(center[::-1].astype(np.int32)),
                 tuple(pc0[::-1].astype(np.int32)),
                 (255,0,0),5)
        cv2.line(res,
                 tuple(center[::-1].astype(np.int32)),
                 tuple(pc1[::-1].astype(np.int32)),
                 (0,255,0),5)
        cv2.imwrite("result.png", res)

    return {"components": [[components[0][0], components[0][1]],
                           [components[1][0], components[1][1]]],
            "center": [center[0], center[1]] }


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
