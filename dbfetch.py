#!/usr/bin/env python
#Author: Brian Cunnane
#Description: Test script to read data from database and output to serial window.
#			  Some kind of exception handling should be implemented.
import MySQLdb
db = MySQLdb.connect("localhost","Brian","password","hive1")
cursor = db.cursor()

cursor.execute("USE hive1;")
cursor.execute("SELECT * FROM DATA;")

print "\nTemperature\tHumidity\tWeight"
print "*************************************************"

for reading in cursor.fetchall():
        print str(reading[0])+"\t\t"+str(reading[1])+"\t\t"+str(reading[2])

db.close()
