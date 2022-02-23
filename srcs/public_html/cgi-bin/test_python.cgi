#!/usr/bin/python -u

# Import modules for CGI handling 
import cgi
import os
# import cgitb
# cgitb.enable(display=0, logdir="./tmp")

# Create instance of FieldStorage 
form = cgi.FieldStorage(
    fp=self.rfile,
    headers=self.headers,
    environ={'REQUEST_METHOD':'POST'})

if "name" not in form or "email" not in form:
    print "<H1>Error</H1>"
    print "Please fill in the name and email fields."
    print "Content-type: text/html\r\n\r\n"
    print "<font size=+1>Environment</font><\br>"
    for param in os.environ.keys():
        print "<b>%20s</b>: %s<\br>" % (param, os.environ[param])
else:
    for param in os.environ.keys():
        print "<b>%20s</b>: %s<\br>" % (param, os.environ[param])
    
    # Get data from fields
    name = form.getvalue('name')
    email  = form.getvalue('email')

    # print "Content-type:text/html\r\n\r\n"
    print "<html>"
    print "<head>"
    print "<title>Hello - Second CGI Program</title>"
    print "</head>"
    print "<body>"
    print "<h1> Test </h1>"
    print "<h2>Hello %s %s</h2>" % (name, email)
    print "</body>"
    print "</html>"
