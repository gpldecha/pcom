import socket
import struct
import selectors
import sys


import threading as thr
from threading import Thread


import multiprocessing
import multiprocessing as mp

import queue
import pickle
import types
import time

class Timer:

    def __init__(self, duration):
        self._duration = duration
        self._start_time = time.time()

    def tick(self):
        if time.time() - self._start_time >= self._duration:
            self._start_time = time.time()
            return True
        else:
            return False

class Publisher(thr.Thread):

    def __init__(self, topic, port, host='localhost', is_verbose=True, use_process=False, single_thread=True):
        thr.Thread.__init__(self, daemon=True)
        self._topic = topic
        self._host = host
        self._port = port
        self._is_verbose = is_verbose
        self._consumer_queues = dict()
        self._use_process = use_process
        if self._use_process:
            self._lock = mp.Lock()
        else:
            self._lock = thr.Lock()
        self._single_thread = single_thread
        self._clients = dict()
        self.start()
        self._send_timer = Timer(1.0)
        self._run_timer = Timer(1.0)
        self._service = Timer(1.0)

    def _apply_lock(self, func):
        self._lock.acquire()
        try:
            func()
        finally:
            self._lock.release()

    def _send_multi_thread(self, data):
        if len(self._consumer_queues) == 0: return
        to_remove = list()
        for key, (client, q) in self._consumer_queues.items():
            if not client.is_alive():
                to_remove.append(key)
                continue
            if q.full():
                try:
                    q.get(timeout=0.1)
                    q.put(data, timeout=0.1)
                except:
                    self._print_msg('unable to get and put data to queue for {}'.format(key))
            else:
                try:
                    q.put(data, timeout=0.1)
                except:
                    self._print_msg('unable to put data to queue for {}'.format(key))
        for key in to_remove:
            ip, port = key.split(':')
            self._remove_client(ip, port)

    def _send_single_thread(self, data):
        to_remove = list()
        for key in list(self._clients):
            connection =
            if not self._send_data(connection, data):
                to_remove.append(key)
        for key in to_remove:
            self._clients.pop(key, None)

    def send(self, data):
        if self._send_timer.tick():
            if self._send_single_thread:
                print(len(self._clients))
            else:
                print(len(self._consumer_queues))
        if self._single_thread:
            self._send_single_thread(data)
            # self._apply_lock(lambda: self._send_single_thread(data))
        else:
            self._apply_lock(lambda : self._send(data))

    def run(self):
        while True:
            try:
                self._run()
            except:
                continue
        self._print_msg('stopped!')

    def _run(self):
        sel = selectors.DefaultSelector()
        pub_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        pub_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        pub_socket.bind((self._host, self._port))
        pub_socket.setblocking(False)
        pub_socket.listen(10)
        sel.register(pub_socket, selectors.EVENT_READ, data=None)
        while True:
            events = sel.select(timeout=0.01)
            if self._run_timer.tick():
                print('select')
            for key, mask in events:
                if key.data is None:
                   self._accept_wrapper(sel, key.fileobj)
                else:
                   self._service_connection(sel, key, mask)
        self._print_msg('run once stopped!')

    def _print_msg(self, msg):
        if self._is_verbose:
            print('[Publisher ' + self._topic + '] msg: ' + msg)

    def _print_connected_clients(self):
        self._print_msg('remaining clients:')
        if len(self._consumer_queues) == 0:
            self._print_msg('\tnone')
        for key in self._consumer_queues:
            self._print_msg(key)

    def _send_data(self, connection, data):
        try:
            payload = pickle.dumps(data)
            header = struct.pack("L", len(payload))
            message = header + payload
        except:
            self._print_msg('client thread unable to serialize data')
        try:
            connection.sendall(message)
        except:
            self._print_msg('client thread unable to send data!, closing for now')
            connection.close()
            return False
        return True

    def _client_thread(self, connection, ip, port, q):
        is_running = True
        while is_running:
            try:
                data = q.get()
            except:
                self._print_msg('client thread exception unable to get data')
                continue
            self._send_data(connection, data)

        self._print_msg('client thread exiting!')

    def _remove_client(self, ip, port):
        key = ip + ':' + str(port)
        p, q = self._consumer_queues.pop(key)
        p.join(timeout=0.1)

    def _accept_wrapper(self, sel, sock):
        conn, addr = sock.accept()
        ip, port = addr
        self._print_msg('accepted connection from: {}'.format(addr))
        conn.setblocking(False)
        data = types.SimpleNamespace(addr=addr, inb=b'', outb=b'')
        events = selectors.EVENT_READ | selectors.EVENT_WRITE
        sel.register(conn, events, data=data)

        def start_thread_process():
            try:
                key = ip + ':' + str(port)
                if key in self._consumer_queues:
                    self._remove_client(ip, port)
                if self._use_process:
                    q = mp.Queue(maxsize=5)
                    p = multiprocessing.Process(target=self._client_thread, args=(conn, ip, port, q), daemon=True)
                else:
                    q = queue.Queue(maxsize=5)
                    p = Thread(target=self._client_thread, args=(conn, ip, port, q), daemon=True)
                self._consumer_queues[key] = (p, q)
                self._print_msg('start the process')
                p.start()
            except:
                self._print_msg('unable to start thread! ')

        def add_to_clients():
            self._clients[addr] = conn

        def start_process():
            if self._single_thread:
                add_to_clients()
                #self._apply_lock(add_to_clients)
            else:
                start_thread_process()

        self._apply_lock(start_process)
        self._print_msg('acceptor done!')

    def _service_connection(self, sel, key, mask):
        sock = key.fileobj
        data = key.data
        if self._service.tick():
            print('service')
        if mask & selectors.EVENT_READ:
            try:
                recv_data = sock.recv(1024)  # Should be ready to read
                if recv_data:
                    data.outb += recv_data
                else:
                    self._print_msg('closing connection to: {}'.format(data.addr))
                    ip, port = data.addr
                    self._apply_lock(lambda: self._remove_client(ip, port))
                    sel.unregister(sock)
                    sock.close()
                    self._apply_lock(lambda: self._print_connected_clients())

            except ConnectionResetError as e:
                self._print_msg('service connection exception: {}'.format(e))
                self._print_msg('closing connection to: {}'.format(data.addr))
                ip, port = data.addr
                self._apply_lock(lambda: self._remove_client(ip, port))
                sel.unregister(sock)
                sock.close()
                self._apply_lock(lambda: self._print_connected_clients())
