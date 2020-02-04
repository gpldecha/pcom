#!/usr/bin/env python3
import socket
import sys
import pickle
import time
import socket
import sys
import traceback
from threading import Thread
import threading
import queue
import pickle

class Consumer(threading.Thread):

    def __init__(self, host, port):
        threading.Thread.__init__(self)
        self.host = host
        self.port = port
        self.queue = queue.Queue(maxsize=2)
        self.start()

    def get_data(self):
        if self.queue.qsize() == 0:
            return None
        else:
            return self.queue.get()

    def run(self):
        soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            soc.connect((self.host, self.port))
        except:
            print("Connection Error: {}:{}".format(self.host, self.port))

        while True:
            msg = soc.recv(5120)
            if self.queue.qsize() >= 2:
                self.queue.get()
            self.queue.put(pickle.loads(msg))

if __name__ == "__main__":
    consumer = Consumer('localhost', 8000)
    while True:
        data = consumer.get_data()
        if data is not None:
            print(data)
        time.sleep(0.5)
