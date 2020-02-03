#!/usr/bin/env python3
import zmq
import sys
import time
import os

context = zmq.Context()
pub = context.socket(zmq.PUB)

USE_ICP = True

if USE_ICP:
    pub.bind("ipc:///tmp/zmq")
else:
    pub.bind("tcp://0.0.0.0:5558")

i=0
while True:
    time.sleep(0.001)
    pub.send(b'A', zmq.SNDMORE)
    #pub.send_string(str(i))
    pub.send_pyobj({'time': i})
    print(i)
    i += 1
