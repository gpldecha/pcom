import socket
import struct
import selectors
import sys


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

class Publisher(mp.Process):

    def __init__(self, topic, port, host='localhost', is_verbose=True):
        mp.Process.__init__(self, daemon=True)
        self._topic = topic
        self._host = host
        self._port = port
        self._is_verbose = is_verbose
        self._lock = mp.Lock()
        self._data_queue = mp.Queue(maxsize=2)
        self.start()

    def _apply_lock(self, func):
        self._lock.acquire()
        try:
            func()
        finally:
            self._lock.release()

    def _push(self, data):
        if self._data_queue.full():
            try:
                self._data_queue.get(timeout=0.1)
            except:
                self._print_msg('push() unable to get and put data to queue')
        try:
            self._data_queue.put(data, timeout=0.1)
        except:
            self._print_msg('push() unable to put data to queue')

    def _pop(self):
        if self._data_queue.empty():
            return None
        else:
            try:
                return self._data_queue.get(timeout=0.1)
            except:
                self._print_msg('pop() unable to get and put data to queue')
                return None

    def _send(self, sel, clients):
        data = self._pop()
        self._send_to_all_clients(data, sel, clients)

    def _send_to_all_clients(self, data, sel, clients):
        if data is None: return
        to_remove = list()

        try:
            payload = pickle.dumps(data)
            header = struct.pack("L", len(payload))
            message = header + payload
        except:
            self._print_msg('client thread unable to serialize data')
            return

        for key, (connection, sock) in clients.items():
            if not self._sendall_data(connection, message):
                to_remove.append(key)
        for key in to_remove:
            clients.pop(key)
            try:
                sock.close()
                print('unregister: {}'.format(key))
            except:
                print('unable to unregister: {}'.format(key))
                continue

    def send(self, data):
        self._lock.acquire()
        try:
            self._push(data)
        finally:
            self._lock.release()

    def run(self):
        sel = selectors.DefaultSelector()
        pub_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        pub_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        pub_socket.bind((self._host, self._port))
        pub_socket.setblocking(False)
        pub_socket.listen(10)
        sel.register(pub_socket, selectors.EVENT_READ, data=None)
        print('start')
        run_timer = Timer(1.0)
        service = Timer(1.0)

        clients = dict()

        while True:
            events = sel.select(timeout=0.05)

            if run_timer.tick():
                print(len(clients))
                print('select')

            for key, mask in events:
                if key.data is None:
                   self._accept_wrapper(sel, key.fileobj, clients)
                else:
                    self._service_connection(sel, key, mask, clients)
            self._apply_lock(lambda:self._send(sel, clients))

        self._print_msg('run once stopped!')

    def _print_msg(self, msg):
        if self._is_verbose:
            print('[Publisher ' + self._topic + '] msg: ' + msg)

    def _print_connected_clients(self):
        self._print_msg('remaining clients:')
        if len(self._clients) == 0:
            self._print_msg('\tnone')
        for key in self._clients:
            self._print_msg(key)

    def _sendall_data(self, connection, message):
        try:
            connection.sendall(message)
        except socket.error as e:
            print(e)
        except:
            self._print_msg('client thread unable to send data!, closing for now')
            connection.close()
            return False
        return True

    def _accept_wrapper(self, sel, sock, clients):
        conn, addr = sock.accept()
        ip, port = addr
        self._print_msg('accepted connection from: {}'.format(addr))
        conn.setblocking(True)
        data = types.SimpleNamespace(addr=addr, inb=b'', outb=b'')
        events = selectors.EVENT_READ | selectors.EVENT_WRITE
        try:
            sel.register(conn, events, data=data)
        except KeyError as e:
            self._print_msg('accept: {}'.format(e))
            conti

        def add_to_clients():
            clients[addr] = (conn, sock)

        add_to_clients()
        # self._apply_lock(add_to_clients)
        self._print_msg('acceptor done!')

    def _service_connection(self, sel, key, mask, clients):
        sock = key.fileobj
        data = key.data
        if mask & selectors.EVENT_READ:
            try:
                recv_data = sock.recv(1024)  # Should be ready to read
                if recv_data:
                    data.outb += recv_data
                else:
                    self._print_msg('closing connection to: {}'.format(data.addr))
                    ip, port = data.addr
                    sel.unregister(sock)
                    sock.close()
                    clients.pop(data.addr)

            except ConnectionResetError as e:
                self._print_msg('service connection exception: {}'.format(e))
                self._print_msg('closing connection to: {}'.format(data.addr))
                ip, port = data.addr
                sel.unregister(sock)
                sock.close()
                clients.pop(data.addr)
