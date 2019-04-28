import signal
import sys

from server import WebServer

def shutdownServer(sig, unused):
  webserver.shutdown()
  sys.exit(1)

signal.signal(signal.SIGINT, shutdownServer)
webserver = WebServer(12000)
webserver.start()
print("Press Ctrl+C to shut down server")