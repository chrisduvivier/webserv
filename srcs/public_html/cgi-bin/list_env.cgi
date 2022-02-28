#!/usr/bin/python -u

import os
print "Content-Type: text/html"
print "Cache-Control: no-cache"
print

print "<html><body>"
for headername, headervalue in os.environ.iteritems():
    if headername.startswith("HTTP_"):
        print "<p>{0} = {1}</p>".format(headername, headervalue)
 
print "</html></body>"