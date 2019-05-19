import os
import smtplib

def prompt(prompt):
    return input(prompt).strip()

print()
print()
print()
print("Welcome to Leland's command line email client! \n")
print("""    _________
   |\       /|
   | \     / |
   |  `...'  |
   |__/___\__|""")
print("") 
to_address = prompt("To: ")
message = prompt("Message: ")
  
# creates SMTP session 
s = smtplib.SMTP('smtp.gmail.com', 587) 
  
# start TLS for security 
s.starttls()
  
# Authentication 
s.login(os.environ["GMAIL"], os.environ["GMAIL_SECRET"]) 
  
#gmail needs you to allow less secure access
#https://myaccount.google.com/lesssecureapps

# sending the mail 
s.sendmail(os.environ["GMAIL"], to_address, message) 
  
# terminating the session 
s.quit() 
