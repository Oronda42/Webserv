import cgi
import http.cookies
import os

# retrieve login variables from the form
form = cgi.FieldStorage()
username = form.getvalue('username')

# print formated username

pouet = [
	f"Set-Cookie:username = {username}",
	"Max-Age = 3600",
	#"Domain = localhost", # No domain so it works on all machines (if no domain is provided, the value will be the host of the url of current document)
	"Path = /",
]

print("; ".join(pouet))
print ("Content-type:text/html\r\n")
print ("<html>")
print ("<head>")
print ("<title>COUCOU</title>")
# print ("<script>")
# print ("window.location.href = 'localhost:8080';")
# print ("</script>")
print ("</head>")
print ("</html>")


