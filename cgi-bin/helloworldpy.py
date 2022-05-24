#!/usr/bin/python

# Import modules for CGI handling 
import cgi, cgitb 

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')

#print(form.keys())

content = [
	"<html>",
	"<head>",
	"<title>Hello - Second CGI Program</title>",
	"</head>",
	"<body>",
	"<h2>Hello {fname} {lname}</h2>".format(fname = first_name, lname = last_name),
	"</body>",
	"</html>"
]

contentLength = sum(len(s) for s in content)

print("Content-Length: {clength}".format(clength = contentLength))
print("Content-type: text/html\r\n\r\n")

for s in content:
	print(s)