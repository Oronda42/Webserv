import requests
import time

# Take hostname as argument
hostname = input("Enter port: ")
while True:
	r = requests.get("http://localhost:" + hostname)
	r = requests.get("http://localhost:" + hostname + "/cgi-bin/helloperl.pl")

	if (r.text.find("Hello Pouet pouet pouet") != -1 and r.status_code == 200):
		print("Server is alive")
	else:
		print("Server is dead")
		break

	# wait 100 ms
	time.sleep(0.01)

print("End")