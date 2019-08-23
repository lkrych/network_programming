# telnet makes a TCP connection with a website and sends commands over plaintext
telnet www.internic.net 80

# when the command prompt shows up type
# HEAD / HTTP/1.0
# to make an HTTP HEAD request to the server

#notice that telnet automatically does a DNS lookup to determine the IP

# SAMPLE OUTPUT
# HTTP/1.1 301 Moved Permanently
# Date: Fri, 23 Aug 2019 14:07:09 GMT
# Server: Apache
# Location: https://www.internic.net/
# Connection: close
# Content-Type: text/html; charset=iso-8859-1