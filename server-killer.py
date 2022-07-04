#!/usr/local/bin/python3

import requests
import time

# Take hostname as argument
hostname = input("Enter port: ")
i = 0
start = time.time()
while True:
	r = requests.get("http://localhost:" + hostname)
	r = requests.get("http://localhost:" + hostname + "/cgi-bin/helloperl.pl?fname=Olivier&lname=Noob")
	# r = requests.get("http://localhost:" + hostname + "/cgi-bin/helloworldpy.py?username=Olivier&password=Ronda+%3C3")

	# if not (r.text.find("Hello Pouet pouet pouet") != -1 and r.status_code == 200):
	# 	print("Wrong response: " + r.text)
	# 	break

	if not (r.text.find("Hello Olivier") != -1 and r.status_code == 200):
		print("Wrong response: " + r.text)
		break

	if (r.status_code != 200):
		print("Wrong response: " + r.text)
		break

	# wait 100 ms
	#time.sleep(0.0001)
	i += 1
	if (i % 50 == 0):
		print("Request count: " + str(i))
		print("Requests per second: " + str(i / (time.time() - start)))

	# Get average of requests per second using i

print("End")