#!/usr/bin/python3 -u

import os
import sys
import urllib.parse

if os.environ.keys():
    if os.environ['QUERY_STRING']:
        query_string = os.environ['QUERY_STRING']

multiform = None
if query_string:
    multiform = urllib.parse.parse_qs(query_string)

print("Content-Type:text/html; charset=utf-8\r\n\r\n")
print('<html>')
print('<head>')
print('<title>Hello World - First CGI Program</title>')
print('</head>')
print('<body>')
print('<h2>Hello World! This is my first CGI program</h2>')
if query_string and multiform and multiform.keys():
    print('<h2> List of the FORM DATA passed to CGI <\h2>')
    for key in multiform.keys():
        print('<p>' + key + '=' + str(multiform[key]) + '</p>')
else:
    print('<h2> FORM DATA passed to CGI was empty! <\h2>')
print('</body>')
print('</html>')