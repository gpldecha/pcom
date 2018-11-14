#!/usr/bin/env python
import sys, os
import time
sys.path.append(os.path.abspath('../../../lib'))
from pypcomtcp import *

class Consumer:

    def __init__(self, host, port, buffer_size=5):
        self._consumer = TCPConsumer(host, port, buffer_size)

    def receive(self):
        return self._consumer.receive()

    def is_alive(self):
        return self._consumer.is_alive()


if __name__ == "__main__":
    print('\nTCP Consumer')

    consumer = Consumer("127.0.0.1", 6066)
    msg = ""

    while True:
        msg = consumer.receive()
        if msg is not None:
            print('got {}'.format(msg))
        if not consumer.is_alive():
            print('not alive anymore')
            consumer = Consumer("127.0.0.1", 6066)
            time.sleep(1.0)
