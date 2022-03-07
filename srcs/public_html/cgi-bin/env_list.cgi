import os

def main():
        
    print('<html>')
    print('<head>')
    print('<title>CGI Program: ENV VAR</title>')
    print('</head>')

    print('<body>')
    print('<h2>Hello World! This is my first CGI program</h2>')
    if os.environ.keys():
        print('<p> List of Enviromental Variables passed to CGI </p>')
        for param in os.environ.keys():
            print("<p> %20s: %s\n </p>" % (param, os.environ[param]))
    else:
        print('<h2> FORM DATA passed to CGI was empty! </h2>')
    print('</body>')
    print('</html>')

if __name__ == "__main__":
    main()