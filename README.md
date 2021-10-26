# 42 Cursus Webserv

Extract from the Project Subject :

" This project is here to make you write your HTTP server. You will be able
to test it with a real browser. HTTP is one of the most used protocols on the internet.
Knowing its arcane will be useful, even if you won’t be working on a website. "

---

Our Webserver
===============

### Overview
This project is fully written in C++. This is a group of project done with two other 42 students.
The server is designed to work with HTTP/1.1 (https://www.ietf.org/rfc/rfc2616.txt) and we based our architecture on Nginx (https://www.nginx.com/resources/library/infographic-inside-nginx/) as we implemented a multi-worker architecture.
The server supports GET HEAD POST PUT DELETE CONNECT and OPTIONS requests as well as PHP-CGI.
The server is serving on localhost:8080/2000/3000. Configuration file can be found in config/webserv.conf

### Our Server architecture

![Alt text](./ressources/Webserver.drawio.png?raw=true)

### Test
This project is designed to work on Linux Ubuntu.
You can test it by simply executing the ubuntu_tester provided in the repositiry or using telnet/curl.

### Contact
trbonnes@student.42.fr
mclaudel@student.42.fr
pganglof@student.42.fr 