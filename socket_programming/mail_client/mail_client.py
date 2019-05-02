import socket
import dns.resolver
from email.utils import parseaddr

def prompt(prompt):
    return input(prompt).strip()

print("Welcome to Leland's command line email client! \n")
print("""    _________
   |\       /|
   | \     / |
   |  `...'  |
   |__/___\__|""")
print("") 
email_input = prompt("To: ")
parsed = parseaddr(email_input) # returns a tuple where second element is email address
domain = ""

if len(parsed[1]) > 0: #the email is validish, some can slip by
  domain = parsed[1].split("@")[1]
else:
  print("That's not a valid email, I will now self destruct.")
  exit(1)

#check if email has an MX record using dnspython
host = ""
try:
  answers = dns.resolver.query(domain, 'MX')
  for rdata in answers:
    print('Host', rdata.exchange, 'has preference', rdata.preference)
    if len(host) == 0:
      host = rdata.exchange
except Exception as e:
  print(e)
  print("There is no MX record for that email and")
  print("unfortunately the resolvers have turned on us. I will now self destruct.")
  exit(1)

#grab a record's host and try to make a TCP connection connection with it

server_name = host 
server_port = 25
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#initialize the tcp connection between the client and server
client_socket.connect((server_name, server_port))

#begin communication with the mail server

# client_socket.send(message.encode())
# modified_message = client_socket.recv(1024)

# print("Message from server: {message}".format(message=modified_message.decode()))

# client_socket.close()

