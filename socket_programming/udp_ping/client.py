import socket
import time

#setup the constants for the client
server_name = 'localhost'
server_port = 12000
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#set timeout for 1 second
client_socket.settimeout(1)

  #begin ping loop 
def main():  
  total_elapsed = 0
  for _ in range(10):
    start = time.time()
    healthy = ping()
    end = time.time()
    elapsed = (end - start) * 1000

    #only print elapsed time for ping if the socket didn't time out
    if healthy:
      print('ping to {server} took {:.3f} ms'.format(elapsed, server=server_name))
      total_elapsed += elapsed

  client_socket.close()

  average_elapsed = total_elapsed/10

  if average_elapsed > 0:
    print('The average ping duration was {:.3f} ms'.format(average_elapsed))
  else:
    print('Ping did not receive any messages from {server}'.format(server=server_name))


#make a UDP ping request to server
def ping():
  #send ping
  client_socket.sendto("ping".encode(), (server_name, server_port))

  #receive pong from server
  try:
    message, _ = client_socket.recvfrom(2048)

    if message.decode() == "pong":
      return True
    else:
      raise Exception

  except:
    print("The socket timed out after 1s. Retrying ping")
    return False

if __name__=="__main__":
   main()