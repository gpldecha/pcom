#!/usr/bin/env python
import sys, os
import time
sys.path.append(os.path.abspath('../../lib'))
from pypcom import *


class Producer:

    def __init__(self, name, array_size, queue_size):
        self._array_size = array_size
        self._producer = ArrayDoubleProducer(name, array_size, queue_size);

    def send(self, data):
        if len(data) != self._array_size:
            return
        self._producer.send(data)


if __name__ == "__main__":
    print('\nProducer')

    producer = Producer("my_data", 1, 5)

    data = [0]
    while True:
        print('send {}'.format(data[0]))
        producer.send(data)
        data[0] += 1
        time.sleep(1.0)
