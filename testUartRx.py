#!/usr/bin/env python
#Author: Brian Cunnane
#Date: 01/Feb/2016
#Description: Script to read data on uart connection.
#             Data to be deliniated by ";"
import serial


ser = serial.Serial("/dev/ttyAMA0")
ser.baudrate = 9600
data = ""
while 1:
        try:
                state=ser.read(1)
                if state.strip() != ";":
                        data += state
                else:
                        print(data)
                        data = ""
