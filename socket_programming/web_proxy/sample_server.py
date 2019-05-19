#simple http server for responding to web proxy
# https://blog.anvileight.com/posts/simple-python-http-server/
from http.server import HTTPServer, BaseHTTPRequestHandler

PORT = 5000

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):

  def do_GET(self):
      self.send_response(200)
      self.end_headers()
      self.wfile.write(b'Hello, world!')

httpd = HTTPServer(('localhost', PORT), SimpleHTTPRequestHandler)
httpd.serve_forever()