import requests
import time

while 1:
    time.sleep(5)
    x = requests.get('http://192.168.1.102:8090/value')
    if x.status_code == 200: 
        value = x.text[:8]
        print(value)
        path = 'http://192.168.1.102:8090/submitCommand?command='
        if value == "D93813C3":
            path += '0' + x.text[8:]
            x = requests.get(path)
        elif value == "14C8662B":
            path += '1' + x.text[8:]
            x = requests.get(path)
