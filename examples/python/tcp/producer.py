#!/usr/bin/env python
import sys, os
import time
sys.path.append(os.path.abspath('../../../lib'))
from pypcomtcp import *


class Producer:

    def __init__(self, port):
        self._producer = TCPProducer(port);

    def send(self, data):
        self._producer.send(data)

if __name__ == "__main__":
    print('\nTCP Producer')

    producer = Producer(6066)

    data = 0
    while True:
        print('send {}'.format(data))
        producer.send(str(data))
        data += 1
        time.sleep(1.0)
