#!/usr/bin/python -u

# Import modules for CGI handling 
import cgi
# import cgitb; cgitb.enable()

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

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