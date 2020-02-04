#!/usr/bin/env python
import zmq
import sys
import time
import pickle

# Socket to talk to server
context = zmq.Context()
sub = context.socket(zmq.SUB)
sub.setsockopt(zmq.RCVHWM, 2)  # This line added.
sub.setsockopt(zmq.SUBSCRIBE, b'')

# sub.setsockopt(zmq.CONFLATE, True)

USE_ICP = False

if USE_ICP:
    sub.connect ("ipc:///tmp/zmq")
else:
    sub.connect ("tcp://0.0.0.0:5558")

while True:

    #msg = sub.recv_multipart()

    topic = sub.recv()
    # data = sub.recv()
    data = sub.recv_pyobj()
    print(data)
    #print(pickle.loads(msg[1]))

    time.sleep(0.5)
