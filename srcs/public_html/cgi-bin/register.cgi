import os
import sys
import urllib.parse

def main():
	multiform = None
	query_string = None
	tmp = None

	if os.environ.keys():		
		if os.environ['REQUEST_METHOD'] == "POST":				#POST => Read from STDIN 
			if os.environ['CONTENT_LENGTH']:
				len = int(os.environ['CONTENT_LENGTH'])
				tmp = sys.stdin.read(len)
				query_string = tmp
		elif os.environ['REQUEST_METHOD'] == "GET":				#GET => Read from QUERY_STRING
			if os.environ['QUERY_STRING']:
				query_string = os.environ['QUERY_STRING']

	if query_string:
		multiform = urllib.parse.parse_qs(query_string)

	# print("Content-Type:text/html; charset=utf-8\r\n\r\n")
	print('<html>')
	print('<head>')
	print('<title>Received Mailing list</title>')
	print('</head>')
	print('<body>')
	print('<h2>Hello World! This is my CGI program reading the user data</h2>')
	if query_string and multiform and multiform.keys():
		print('<h3> List of the FORM DATA passed to CGI </h3>')
		for key in multiform.keys():
			print('<p>' + key + ' = ' + str(multiform[key][0]) + '</p>')
	else:
		print('<h3> FORM DATA passed to CGI was empty! </h3>')
	print('</body>')
	print('</html>')

if __name__ == "__main__":
    main()