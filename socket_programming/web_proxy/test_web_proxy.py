#send multiple requests into the server to test its ability to multithread
import requests 

#send 50 requests
for i in range(50):
  r = requests.get('http://127.0.0.1:5000') 
  print("request {i}: {data}".format(i=i, data=r.content))