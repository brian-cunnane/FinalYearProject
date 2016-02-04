#!/usr/bin/env python

import MySQLdb

db = MySQLdb.connect("localhost", "monitor","password","testing")
cursor = db.cursor()

try:
        cursor.execute ("INSERT INTO testdata values(CURRENT_DATE(),'10:40')")

        db.commit()
        print "Data committed"

except:
        print "Error inserting data. Rolling back database"
        db.rollback
