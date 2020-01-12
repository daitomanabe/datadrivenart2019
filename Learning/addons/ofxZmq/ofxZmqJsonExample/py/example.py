#!/usr/bin/env python

import time
import zmq

import numpy as np
import codecs, json

context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind('ipc:///tmp/numpy_json')

while True:
	time.sleep(1)
	arr = np.random.rand(3, 2, 2)
	json_str = json.dumps(arr.tolist())
	socket.send_string(json_str)
	print(json_str)