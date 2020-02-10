import socket
import struct
from enum import Enum
import time
import multiprocessing
import multiprocessing as mp
import pickle


class SOCKET_READ_STATE(Enum):
    NO_DATA=0
    ERROR=1
    DATA=2


class Consumer(multiprocessing.Process):

    def __init__(self, topic, port, host='localhost', is_verbose=True):
        multiprocessing.Process.__init__(self, daemon=True)
        self._topic = topic
        self._host = host
        self._port = port
        self._is_verbose = is_verbose
        self._data_queue = mp.Queue(maxsize=2)
        self.start()

    def get_data(self):
        if self._data_queue.empty():
            return None
        else:
            try:
                return self._data_queue.get(timeout=0.01)
            except:
                return None

    def run(self):

        while True:
            self._print_msg('trying to connect')
            soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            try:
                soc.connect((self._host, self._port))
            except:
                self._print_msg('connection Error: {}:{}'.format(self._host, self._port))
            soc.settimeout(5)
            self._read(soc)
            soc.close()

    def _print_msg(self, msg):
        if self._is_verbose:
            print('[Consumer ' + self._topic + '] msg: ' + msg)

    def _read(self, socket):
        data = [b'']
        payload_size = struct.calcsize("L")
        time_last_data = [time.time()]
        while True:
            output = [None, SOCKET_READ_STATE.NO_DATA, None]
            while len(data[0]) < payload_size:
                self._read_socket(socket, output)
                if not self._update_data(output, data, time_last_data):
                    return

            packed_msg_size = data[0][:payload_size]
            data[0] = data[0][payload_size:]
            msg_size = struct.unpack("L", packed_msg_size)[0]

            while len(data[0]) < msg_size:
                self._read_socket(socket, output)
                if not self._update_data(output, data, time_last_data):
                    return

            frame_data = data[0][:msg_size]
            data[0] = data[0][msg_size:]

            try:
                frame=pickle.loads(frame_data)
            except:
                data = [b'']
                self._print_msg('unable to unpickle message')
                continue

            if frame is not None:
                try:
                    self._data_queue.put(frame, timeout=0.01)
                except:
                    pass

                # if self._data_queue.full():
                #     try:
                #         self._data_queue.get(timeout=None)
                #         self._data_queue.put(frame, timeout=None)
                #     except:
                #         pass
                # else:
                #     try:
                #         self._data_queue.put(frame, timeout=None)
                #     except:
                #         pass

    def _read_socket(self, socket, output):
        try:
            output[0] = socket.recv(1024)
        except OSError as e:
                output[0] = None
                output[1] = SOCKET_READ_STATE.ERROR
        except socket.timeout as e:
                output[0] = None
                output[1] = SOCKET_READ_STATE.NO_DATA
        except socket.error as e:
            output[0] = None
            output[1] = SOCKET_READ_STATE.ERROR
        else:
            if len(output[0]) == 0:
                output[0] = None

    def _update_data(self, _output, data, time_last_data):
        if _output[0] is not None:
            data[0] += _output[0]
            time_last_data[0] = time.time()
            return True
        elif time.time() - time_last_data[0] > 4.0:
            return False
        else:
            return True
