import sys
import cgi, os
import cgitb; cgitb.enable()
import urllib.parse as urlparse

def main():
	
	simpleform = None
	query_string = None
	if os.environ.keys():
		if os.environ['QUERY_STRING']:
			query_string = os.environ['QUERY_STRING']

	if query_string:
		simpleform = urlparse.parse_qs(query_string)

	# A nested FieldStorage instance holds the file
	fileitem = simpleform['file'][0]

	sys.stderr.write("fileitem: ")
	sys.stderr.write(fileitem)

	# Test if the file was uploaded
	if fileitem:
		# strip leading path from file name
		# to avoid directory traversal attacks
		fn = os.path.basename(fileitem)
		open('/upload/' + fn, 'wb').write(fileitem.read())
		message = 'The file "' + fn + '" was uploaded successfully'

	else:
		message = 'No file was uploaded'

	print("""\
	Content-Type: text/html\n
	<html><body>
	<p>%s</p>
	</body></html>
	""" % (message,))

if __name__ == "__main__":
	main()