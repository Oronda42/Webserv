#!/usr/bin/python3
import cgi, sys, os
import cgitb; cgitb.enable()
form = cgi.FieldStorage()
sys.path.insert(0, os.getcwd())

message = None

if 'filename' in form:
	fileitem = form['filename']
	fn = os.path.basename(fileitem.filename)
	if (fn == ""):
		message = 'No file was uploaded'
	else:
		# Retrive env from UPLOAD_DIRECTORY
		uploadDir = os.environ['UPLOAD_DIRECTORY']
		if (uploadDir == ""):
			uploadDir = './uploads'

		# Add / at the end if none is present
		if (uploadDir[-1] != '/'):
			uploadDir += '/'

		if not os.path.exists(uploadDir):
			os.makedirs(uploadDir)
		open(uploadDir + fn, 'wb').write(fileitem.file.read())
		message = 'The file \'' + fn + '\' was uploaded successfully to ' + uploadDir
else:
	message = 'No file was uploaded'

print ("Content-type:text/html\r\n")
print ("<html>")
print ("<head>")
print("<meta charset=\"UTF-8\">")
print("<script type=\"text/javascript\">")
print("window.location.href = \"/bonuses.html\"")
print(f"alert(\"{message}\")")
print("</script>")
print ("<title>Upload</title>")
print ("</head>")
print ("</html>")