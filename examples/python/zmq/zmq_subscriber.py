#!/usr/bin/env python3
import zmq
import sys
import time

# Socket to talk to server
context = zmq.Context()
subscriber = context.socket(zmq.SUB)
subscriber.connect ("tcp://localhost:5558")
#subscriber.connect("ipc://example")
subscriber.setsockopt(zmq.SUBSCRIBE, b'B')

while True:

    topic = subscriber.recv()
    data = subscriber.recv_json()
    print(data['time'])
    print(data['position'])
    print(data['velocity'])

    time.sleep(0.01)
