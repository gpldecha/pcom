#!/usr/bin/env python3
import cv2
import time
from socket_publisher import Publisher
from setproctitle import setproctitle
import numpy as np

TEST_CASE = 2

if __name__ == "__main__":

    setproctitle('webcam_publisher')


    publisher = Publisher('webcam', 8001, is_verbose=True)
    cap = cv2.VideoCapture(0)
    frame = None
    encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 50]
    i=0
    while True:

        ret, frame = cap.read()
        _, frame = cv2.imencode('.JPEG', frame, encode_param)
        publisher.send({'time': i, 'image': frame})
        i += 1

        #time.sleep(0.01)

        # if TEST_CASE == 2:
        #     time.sleep(0.005)

    print('Finish')
