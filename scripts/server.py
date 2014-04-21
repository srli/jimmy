#!/usr/bin/env python 

""" 
A simple echo server 
""" 

import socket 

host = "208.91.53.169"
port = 50000 
backlog = 5 
size = 1024 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.bind((host,port)) 
s.listen(backlog) 
while 1: 
    client, address = s.accept() 
    data = client.recv(size)
    while data:
        message = str(raw_input("What should Jimmy say?   "))
        client.send(message) 
        print "Data sent!"
client.close()