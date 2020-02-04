#!/usr/bin/env python3
import socket
import sys
import traceback
from threading import Thread
import threading
import queue
import pickle

class Publiser(threading.Thread):

    def __init__(self, host, port):
        threading.Thread.__init__(self)
        self.host = host
        self.port = port
        self.queue = queue.Queue(maxsize=2)
        self.start()

    def send(self, data):
        if self.queue.qsize() >= 2:
            self.queue.get()
        self.queue.put(data)

    def run(self):
        soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        soc.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        soc.bind((self.host, self.port))
        soc.listen(5)
        while True:
            connection, address = soc.accept()
            ip, port = str(address[0]), str(address[1])
            print("Connected with " + ip + ":" + port)
            try:
                Thread(target=self._client_thread, args=(connection, ip, port)).start()
            except:
                print("Thread did not start.")
        soc.close()

    def _client_thread(self, connection, ip, port):
        is_active=True
        while is_active:
            try:
                data = self.queue.get()
                msg = pickle.dumps(data)
                connection.sendall(msg)
            except:
                connection.close()
                print("Connection " + ip + ":" + port + " closed")
                is_active = False

if __name__ == "__main__":
    import time
    publisher = Publiser('localhost', 8000)
    i=0
    while True:
        publisher.send({'data': i})
        i += 1
        time.sleep(0.001)
