#!/usr/bin/env python
import sys, os
import time
sys.path.append(os.path.abspath('../../../lib'))
from pypcomipc import *

class Consumer:

    def __init__(self, name, array_size):
        self._consumer = ArrayDoubleConsumer(name, array_size)

    def receive(self):
        return self._consumer.receive()

if __name__ == "__main__":
    print('\nConsumer')

    consumer = Consumer("my_data", 1)

    while True:
        data = consumer.receive()
        if len(data) > 0:
            print('got {}'.format(data[0]))
            time.sleep(1.0)
