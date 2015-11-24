#!/usr/bin/env python

import MySQLdb
db = MySQLdb.connect("localhost","monitor","password","testing")
cursor = db.cursor()

cursor.execute("USE testing;")
cursor.execute("SELECT * FROM testdata;")

print "\nDate           Time"
print "====================="

for reading in cursor.fetchall():
        print str(reading[0])+"      "+str(reading[1])

db.close()
