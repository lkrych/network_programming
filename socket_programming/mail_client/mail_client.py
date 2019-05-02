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
      host = rdata.exchange.to_text(omit_final_dot=True)
except Exception as e:
  print(e)
  print("There is no MX record for that email and")
  print("unfortunately the resolvers have turned on us. I will now self destruct.")
  exit(1)

#grab a record's host and try to make a TCP connection connection with it

server_name = host 
server_port = 25
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


client_socket.connect((server_name, server_port))

#begin communication with the mail server
# sample SMTP communication

# S: 220 <HOST DOMAIN>
# C: HELO <CLIENT DOMAIN>
# S: 250 Hello <CLIENT DOMAIN>, pleased to meet you
# C: MAIL FROM: <leland.krych@gmail.com>
# S: 250 leland.krych@gmail.com ... Sender ok
# C: RCPT TO: <blah@example.com>
# S: 250 blah@example.com ... Recipient ok
# C: DATA
# S: 354 Enter mail, end with "." on a line by itself
# C: Here is a message
# C: .
# S: 250 Message accepted for delivery
# C: QUIT
# S: 221 <HOST DOMAIN> closing connection 
greeting_from_server = client_socket.recv(1024)
if greeting_from_server[:3] != '220':
    print('220 reply not received from server. received', greeting_from_server.decode())
    print('I cannot go on. I am embarassed. I will now self-destruct :(')
    exit(1)
print("Greeting from mail server: ", greeting_from_server.decode())

client_socket.send("HELO Leland's Simple Mail Client")

handshake_from_server = client_socket.recv(1024)
if handshake_from_server[:3] != '250':
    print('250 reply not received from server. received', handshake_from_server.decode())
    print('The mail server doesn\'t want to shake hands. I am embarassed.')
    print('I will now self-destruct.')
    exit(1)
print("Handshake from mail server: ", handshake_from_server.decode())

client_socket.close()

