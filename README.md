# Webserv
This project consists of making our own HTTP server. The end product will handle GET/POST/DELETE requests, handle different ports, some simple CGI (only script in python3 in our case), and remains available (+99.5%) with siege stress test. Details of the subject can be found in `wesbserv_subject.pdf`. The projects runs on Mac OS/Linux. Code needed to comply with the C++ 98 standard (No external library and Boost libraries).

## Credit:
Thanks to: [@chrisduvivier](https://github.com/chrisduvivier)  [@Kovlin](https://github.com/Kovlin)    [@Sagaille](https://github.com/Sagaille)


## Run
Usage: `./webserv [configuration file (optional)]`
Run `make` inside srcs directory. The executable `webserv` takes the `default.conf` as parameter if no conf file is provided. The PORT listening by default is 8050 and 8020.
Once the webserv running, open on your browser [here](http://localhost:8050) (we recommend Safari or Google Chrome.)
Tester: `srcs/tester.sh` provides some test to check basic functionalilty.

## Starting point (main resources)
- Read fantastic [guide](https://www.notion.so/Documentation-Webserv-320727979ffd4176a7dd5ba41aaadf46) made by former 42 students (written in French).
- [How to build a web client (Jacob Sorber Youtube)](https://www.youtube.com/watch?v=bdIiTxtMaKA&list=PL9IEJIKnBJjH_zM5LnovnoaKlXML5qh17&index=1)
- [How to build a web server (Jacob Sorber Youtube)](https://www.youtube.com/watch?v=esXw4bdaZkc&list=PL9IEJIKnBJjH_zM5LnovnoaKlXML5qh17&index=2)
- One of the most detailed example program for non-blocking select: [example of IBM here](https://www.ibm.com/docs/en/i/7.2?topic=designs-example-nonblocking-io-select)
- [Starting with HTTP Request](https://docstore.mik.ua/orelly/linux/cgi/ch02_01.htm)
- [Starting with CGI](https://docstore.mik.ua/orelly/linux/cgi/ch03_01.htm)


## more notes and ressources
- [Motivation for using "multiplexing"](https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/#.Yfm19epKiHs)
- [What is www and public_html directory in my folder?](https://www.interserver.net/tips/kb/what-is-the-www-and-publichtml-directory-in-my-folder/): www is a symlink to the `public_html` directory. 
- [public_html]: The public_html directory is the folder to place in web-readable files.
- About Multiplexing: [Select vs Poll](https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/?sfw=pass1639486423#.YbiT3BNKjUI) -> In a nutshell: poll is more efficient than select. select is more portable, as some Unix systems do not support poll.
- Guide to Networking: [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/#pollman) -> maybe use it as a reference book.
- [DWise1's Sockets Programming Pages Dealing With and Getting Around Blocking Sockets] (http://dwise1.net/pgm/sockets/blocking.html#NONBLOCK)
- [Intro to Common Gateway Interface (CGI)](https://www.uregina.ca/science/cs/resources/create-personal-home-page/common-gateway-interface.html)
- [HTTP CGI Programming](https://forhjy.medium.com/42-webserv-cgi-programming-66d63c3b22db)
- [Things to remember when writing CGI scripts](http://astroa.physics.metu.edu.tr/MANUALS/cgi_perl.tut/basics.html)

## Getting on the HTTP Messages:
- Find out how there are defined: https://www.rfc-editor.org/rfc/rfc2616#section-4.1
- Http Request: https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages#http_requests
- Status code: https://developer.mozilla.org/en-US/docs/Web/HTTP/Status

### Flow of development:
Our starting point was Jacob Sorber's tutorial on web client and web server (see the 2 links above). We immplemented ourselves, and we tried to get the request (a simple "Hello World" string) on our server, sent from our client. Next, find the necessary header (we then searched for HTML 1.1 Messages on RFC, a huge documentation about the HTML protocol), and make a parser of HttpRequest for a GET. Next was making the HttpResponse for GET. Next was POST. Then, implemented CGI (checked ressources listed above). Configuration file parsing -> Support for multiple server (multiple PORT) was added. 