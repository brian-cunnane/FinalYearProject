#!/usr/bin/env python
#Author: Brian Cunnane
#Date: 06/Feb/2016
#Description: Script to read data on uart connection.
#             Data to be inserted in mysql DB
#             Data to be deliniated by ";"
#             A single character will be transmitted and two additional dummy values will be inserted into the db.
import serial
import MySQLdb

ser = serial.Serial("/dev/ttyAMA0")
ser.baudrate = 9600
db = MySQLdb.connect("localhost","Brian","password","hive1")
cursor = db.cursor()
data = ""
while 1:
        try:
                state=ser.read(1)
                if state.strip() != ";": # check for end of data
                        print("Reading data")
                        data += state # build string
                else:
                        print(data)
                        temp = int(data) # convert the reading to an int
                        humidity = 0
                        weight = 0;

                        cursor.execute("INSERT INTO DATA VALUES(%s,%s,%s)",(temp,humidity,weight))
                        db.commit()
                        data = ""
                        print("Committed")
        except MySQLdb.Error as DBIE:
                print(DBIE)
                db.rollback
                pass
ser.close()
db.close()
