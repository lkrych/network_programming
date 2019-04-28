import socket
import sys
import time

#borrowed heavily from https://gist.github.com/joncardasis/cc67cfb160fa61a0457d6951eff2aeae

class WebServer(object):
  """
  class for a simple http web server
  """
  def __init__(self, port=8080):
    self.host = 'localhost'
    self.port = port
    self.content_dir = 'files_for_server'

  def start(self):
    self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
      print("Starting server on {host}:{port}".format(host=self.host, port=self.port))
      self.socket.bind((self.host, self.port))
      print("Server started on port {port}".format(port=self.port))
    
    except Exception as e:
      print("Error: could not bind to port {port}".format(port=self.port))
      self.shutdown()
      sys.exit(1)
    
    self._listen()

  def shutdown(self):
    try:
      print("Shutting down the server")
      self.socket.shutdown(socket.SHUT_RDWR)
    
    except Exception as e:
      pass #pass if socket is already closed

  def _listen(self):
    self.socket.listen(1) #only allow one connection
    while True:
      (client, address) = self.socket.accept()
      client.settimeout(60)
      print("Received connection from {addr}".format(addr=address))
      self._handle_client(client, address)
  
  def _handle_client(self, client, address):
    # main loop for handling serving files from files_for_server
    PACKET_SIZE = 1024
    while True:
      print("Client", client)
      data = client.recv(PACKET_SIZE).decode() # receive data packet

      if not data: break

      request_method = data.split(' ')[0]
      print("Method: {m}".format(m=request_method))
      print("Request Body: {b}".format(b=data))

      if request_method == "GET" or request_method == "HEAD":
        # "GET /hello.html"
        file_requested = data.split(' ')[1]

        #ignore parameters
        file_requested = file_requested.split('?')[0]

        if file_requested == "/":
          file_requested = "/index.html"

        filepath_to_serv = self.content_dir + file_requested
        print("Serving web page [{fp}]".format(fp=filepath_to_serv))

        # Load and serve file content
        try:
          f = open(filepath_to_serv, 'rb')
          if request_method == "GET":
            response_data = f.read()
          f.close()
          response_header = self._generate_headers(200)
      
        except Exception as e:
          print("File not found. Serving 404 page.")
          response_header = self._generate_headers(404)

          if request_method == "GET":
            response_data = "<html><body><center><h1>Error 404: File not found</h1></center><p>Go to <a href=\"/\">home</a>.</p></body></html>"
            response_data = response_data.encode()

        response = response_header.encode()
        if request_method == "GET":
          response += response_data
        

        #use socket received client to send back html
        client.send(response)
        client.close()
        break
      else:
        print("Unknown HTTP request method: {method}".format(method=request_method))
  
  def _generate_headers(self, response_code):
    header = ''
    if response_code == 200:
      header += 'HTTP/1.1 200 OK\n'
    elif response_code == 404:
      header += 'HTTP/1.1 404 Not Found\n'
    
    time_now = time.strftime("%a, %d %b %Y %H:%M:%S", time.localtime())
    header += 'Date: {now}\n'.format(now=time_now)
    header += 'Server: Leland\'s Janky Python Server\n'
    header += 'Last-Modified: {now}\n'.format(now=time_now)
    header += 'Content-Type: text/html\n'
    header += 'Connection: close\n\n' # Signal that connection will be closed after completing the request
    return header
