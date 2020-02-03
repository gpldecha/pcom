#!/usr/bin/env python3
import zmq
import sys
import time

# Socket to talk to server
context = zmq.Context()
sub = context.socket(zmq.SUB)
sub.setsockopt(zmq.SUBSCRIBE, b'A')
sub.setsockopt(zmq.CONFLATE, True)

USE_ICP = True

if USE_ICP:
    sub.connect ("ipc:///tmp/zmq")
else:
    sub.connect ("tcp://0.0.0.0:5558")

while True:

    topic = sub.recv()
    # data = sub.recv()
    data = sub.recv_pyobj()

    print(data)

    time.sleep(0.5)
