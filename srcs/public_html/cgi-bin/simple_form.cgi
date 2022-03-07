import os
import sys
import urllib.parse


def main():
		
	simpleform = None
	query_string = None
	if os.environ.keys():
		if os.environ['QUERY_STRING']:
			query_string = os.environ['QUERY_STRING']
		
		# if os.environ['REQUEST_METHOD'] == "POST":				#POST => Read from STDIN 
																	# -> couldn't so the workaround in mucase is to read from querystring as well
		# 	if os.environ['CONTENT_LENGTH']:
		# 		len = int(os.environ['CONTENT_LENGTH'])
		# 		print(sys.stdin.read(len))
		# elif os.environ['REQUEST_METHOD'] == "GET":				#GET => Read from QUERY_STRING
		# 	if os.environ['QUERY_STRING']:
		# 		query_string = os.environ['QUERY_STRING']

	if query_string:
		simpleform = urllib.parse.parse_qs(query_string)

	# print("Content-Type:text/html; charset=utf-8\r\n\r\n")
	print('<html>')
	print('<head>')
	print('<title>Simple Form OUTPUT</title>')
	print('</head>')
	print('<body>')
	print('<h2>We accepted your form!</h2>')
	if query_string and simpleform and simpleform.keys():
		print('<h3> List of the FORM DATA passed to CGI </h3>')
		for key in simpleform.keys():
			print('<p>' + "Your favorite game is" + ': ' + str(simpleform[key]) + '</p>')
	else:
		print('<h3> ...But the form was empty! Please submit some input in the form!  </h3>')
	print('</body>')
	print('</html>')

if __name__ == "__main__":
    main()