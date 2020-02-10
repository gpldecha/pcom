#!/usr/bin/env python3
import time
import cv2
from socket_consumer import Consumer
import argparse
from setproctitle import setproctitle
import numpy as np

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("--name")
    args = parser.parse_args()
    setproctitle("webcam_subscriber_" + str(args.name))

    DISPLAY_VIDEO = True
    consumer = Consumer(topic=args.name, port=8001)
    count = 0
    # if DISPLAY_VIDEO:
    #
    #     cv2.namedWindow('consumer_' + str(args.name),cv2.WINDOW_NORMAL)
    frequency = 0
    s_time = time.time()
    frame = np.zeros((480, 640, 3), dtype=np.uint8)
    while True:
        data = consumer.get_data()
        if data is not None:
            count += 1
            if time.time() - s_time >= 1.0:
                frequency = count/1.0
                #print('time {} frequency {}'.format(data['time'], count))
                count = 0
                s_time = time.time()
            frame = data['image']
            #frame = cv2.imdecode(frame, 1)
            cv2.putText(frame,str(frequency) + ' fps', (20,50), cv2.FONT_HERSHEY_SIMPLEX, 1.5,(255,0,0),3,cv2.LINE_AA)
            cv2.imshow('consumer_' + str(args.name), frame)
            cv2.waitKey(1)


        # if data is not None and DISPLAY_VIDEO:
        #     #frame = data['image']
        #     #frame = cv2.imdecode(frame, 1)
        #     # frame.fill(0)
        #     # cv2.putText(frame,str(frequency) + ' fps', (20,50), cv2.FONT_HERSHEY_SIMPLEX, 1.5,(255,0,0),3,cv2.LINE_AA)
        #     # cv2.imshow('consumer_' + str(args.name), frame)
        #     # cv2.waitKey(1)
        #     count += 1
        #     if time.time() - s_time >= 1.0:
        #         frequency = count/1.0
        #         print('frequency: {}'.format(count))
        #         count = 0
        #         s_time = time.time()
