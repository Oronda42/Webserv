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
print("<meta charset=\"UTF-8\">")
print("<script type=\"text/javascript\">")
print("window.location.href = \"/\"")
print("</script>")
print("<title>Page Redirection</title>")
print ("<title>COUCOU</title>")
print ("</head>")
print ("</html>")


