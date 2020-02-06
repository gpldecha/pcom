#!/usr/bin/env python3
import socket
import struct
import selectors
import sys
import traceback
from threading import Thread
import threading
import queue
import pickle
import types
import numpy as np
import cv2

class Publiser(threading.Thread):

    def __init__(self, host, port):
        threading.Thread.__init__(self)
        self.host = host
        self.port = port
        self.queues = dict()
        self.deamon=True
        self.start()

    def send(self, data):
        if len(self.queues) == 0: return
        for _, q in self.queues.items():
            if q.qsize() >= 2:
                q.get()
            q.put(data)

    def accept_wrapper(self, sel, sock):
        conn, addr = sock.accept()  # Should be ready to read
        ip, port = addr
        print('accepted connection from', addr)
        conn.setblocking(False)
        data = types.SimpleNamespace(addr=addr, inb=b'', outb=b'')
        events = selectors.EVENT_READ | selectors.EVENT_WRITE
        sel.register(conn, events, data=data)
        try:
            self.queues[ip + ':' + str(port)] = queue.Queue(maxsize=2)
            Thread(target=self._client_thread, args=(conn, ip, port)).start()
        except:
            print('Unable to start thread')

    def service_connection(self, sel, key, mask):
        sock = key.fileobj
        data = key.data
        if mask & selectors.EVENT_READ:
            try:
                recv_data = sock.recv(1024)  # Should be ready to read
                if recv_data:
                    data.outb += recv_data
                else:
                    print('closing connection to', data.addr)
                    ip, port = data.addr
                    self.queues.pop(ip + ':' + str(port), None)
                    sel.unregister(sock)
                    sock.close()
                    print('Remaining clients connected')
                    for key in self.queues:
                        print(key)
            except ConnectionResetError as e:
                print('service connection excepion: {}'.format(e))
                print('closing connection to', data.addr)
                ip, port = data.addr
                self.queues.pop(ip + ':' + str(port), None)
                sel.unregister(sock)
                sock.close()
                print('Remaining clients connected')
                for key in self.queues:
                    print(key)

        if mask & selectors.EVENT_WRITE:
            if data.outb:
                print('echoing', repr(data.outb), 'to', data.addr)
                sent = sock.send(data.outb)  # Should be ready to write
                data.outb = data.outb[sent:]


    def run(self):
        sel = selectors.DefaultSelector()
        pub_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        pub_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        pub_socket.bind((self.host, self.port))
        pub_socket.setblocking(0)
        pub_socket.listen(5)
        sel.register(pub_socket, selectors.EVENT_READ, data=None)

        while True:
            events = sel.select(timeout=None)
            for key, mask in events:
                if key.data is None:
                    self.accept_wrapper(sel, key.fileobj)
                else:
                    self.service_connection(sel, key, mask)

    def _client_thread(self, connection, ip, port):
        is_active=True
        q = self.queues[ip + ":" + str(port)]
        while is_active:
            try:
                data = q.get()
                payload = pickle.dumps(data)
                header = struct.pack("L", len(payload))
                message = header + payload
                connection.sendall(message)
            except:
                connection.close()
                is_active = False


if __name__ == "__main__":
    import time
    publisher = Publiser('localhost', 8001)
    cap = cv2.VideoCapture(0)
    i=0
    while True:
        ret, frame = cap.read()
        publisher.send({'time': i, 'image': frame})
        i += 1
