#!/usr/local/bin/python3

import requests
import time

# Take port as argument
port = input("Enter port: ")
i = 0
start = time.time()
while True:
	request = requests.get(f"http://localhost:{port}/")
	request = requests.get(f"http://localhost:{port}/cgi/helloperl.pl?fname=Olivier&lname=Noob")

	if (request.status_code != 200):
		print("Wrong response: " + request.text)
		break

	i += 1
	if (i % 50 == 0):
		print("Request count: " + str(i))
		print("Requests per second: " + str(i / (time.time() - start)))

print("End")
