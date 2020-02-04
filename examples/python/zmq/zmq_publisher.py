#!/usr/bin/env python
import zmq
import sys
import time
import os

context = zmq.Context()
pub = context.socket(zmq.PUB)
pub.setsockopt(zmq.SNDHWM, 2)

USE_ICP = False

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
