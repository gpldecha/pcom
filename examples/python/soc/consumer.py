#!/usr/bin/env python3
import socket
import sys
import time
import socket
import multiprocessing
import multiprocessing as mp
import pickle
import struct
import cv2
import time

class Consumer(multiprocessing.Process):

    def __init__(self, host, port):
        multiprocessing.Process.__init__(self)
        self.host = host
        self.port = port
        self.queue = mp.Queue(maxsize=2)
        self.start()

    def get_data(self):
        if self.queue.empty():
            return None
        else:
            try:
                return self.queue.get(timeout=0.01)
            except multiprocessing.queues.Empty:
                return None
            except:
                return None

    def run(self):
        soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            soc.connect((self.host, self.port))
        except:
            print("Connection Error: {}:{}".format(self.host, self.port))

        data = b''
        payload_size = struct.calcsize("L")
        while True:
            while len(data) < payload_size:
                data += soc.recv(4096)
            packed_msg_size = data[:payload_size]

            data = data[payload_size:]
            msg_size = struct.unpack("L", packed_msg_size)[0]

            while len(data) < msg_size:
                data += soc.recv(4096)
            frame_data = data[:msg_size]
            data = data[msg_size:]

            frame=pickle.loads(frame_data)

            if frame is not None:
                if self.queue.full():
                    self.queue.get(timeout=0.1)
                try:
                    self.queue.put(frame, timeout=0.1)
                except:
                    pass


if __name__ == "__main__":
    consumer = Consumer('localhost', 8001)
    count = 0
    s_time = time.time()
    cv2.namedWindow('consumer',cv2.WINDOW_NORMAL)
    while True:
        data = consumer.get_data()
        if data is not None:
            cv2.imshow('consumer', data['image'])
            cv2.waitKey(1)
            count += 1
            if time.time() - s_time >= 1.0:
                print(count)
                count = 0
                s_time = time.time()
