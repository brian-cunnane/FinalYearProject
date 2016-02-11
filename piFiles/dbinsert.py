#!/usr/bin/env python
#Author: Brian Cunnane
#Description: Test script to insert dummy readings into database;
#			  Exception handling needs to be improved.

import MySQLdb

print("connecting")
db = MySQLdb.connect("localhost", "Brian","password","hive1")
cursor = db.cursor()

try:
        print("inserting")
        cursor.execute ("INSERT INTO DATA VALUES(28,100,60)")

        db.commit()
        print "Data committed"

except:
        print "Error inserting data. Rolling back database"
        db.rollback
