# webserv
42 webserv project

# Starting point (main resources)
- Read fantastic [guide](https://www.notion.so/Documentation-Webserv-320727979ffd4176a7dd5ba41aaadf46) made by former 42 students (written in French). Some take away from this guide:
- [How to build a web client](https://www.youtube.com/watch?v=bdIiTxtMaKA&list=PL9IEJIKnBJjH_zM5LnovnoaKlXML5qh17&index=2) (Our `tcpclient.c` in the POC/mini_client folder is based on this video)
- [How to build a web server]: see POC in the documentation
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
- [Things to remember when writing CGI scripts](http://astroa.physics.metu.edu.tr/MANUALS/cgi_perl.tut/basics.html)



## Getting on the HTTP Messages:
- Find out how there are defined: https://www.rfc-editor.org/rfc/rfc2616#section-4.1
- Http Request: https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages#http_requests
- Status code: https://developer.mozilla.org/en-US/docs/Web/HTTP/Status

## TODOs: Server Side
- [x] create a server-like program that listen for HTML request.
- [ ] make the server setup ports and maybe other things like servername FROM a config file (like nginx) 
- [x] Define a HttpRequest(HttpMessage) class and create it when reading client request/prepare response to the client.
- [x] Make a HttpResponse class which takes the request as input and build response.
- [x] Be able to output a html file on browser via typing `localhost::PORT/URL`

## TODOs: Client Side/Tester
- [x] Simple socket that sends request to the server

## Status Code
- `GET`: indicates that a resource should be fetched.
- `POST`: data is pushed to the server (creating or modifying a resource, or generating a temporary document to send back).