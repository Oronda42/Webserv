import requests
import time

# Take hostname as argument
hostname = input("Enter port: ")
i = 0
start = time.time()
while True:
	r = requests.get("http://localhost:" + hostname)
	r = requests.get("http://localhost:" + hostname + "/cgi-bin/helloperl.pl")

	if not (r.text.find("Hello Pouet pouet pouet") != -1 and r.status_code == 200):
		print("Server is dead")
		break

	# wait 100 ms
	time.sleep(0.0001)
	i += 1
	if (i % 50 == 0):
		print("Request count: " + str(i))
		print("Requests per second: " + str(i / (time.time() - start)))

	# Get average of requests per second using i

print("End")