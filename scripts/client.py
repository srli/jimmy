#!/usr/bin/env python 

""" 
A simple echo client 
""" 

import socket 

host = "208.91.53.169"
port = 50000 
size = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.connect((host,port)) 
while(1):
    s.send('test string to ensure client is waiting') 
    data = s.recv(size) 
    print 'Received:', data
    if data == "end":
        s.close() 
