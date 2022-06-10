#!/usr/bin/python3
import cgi, sys, os
import cgitb; cgitb.enable()
form = cgi.FieldStorage()
sys.path.insert(0, os.getcwd())

message = None

# Test if the file is loaded for the upload
if form['filename'] == '':
	print("suce")
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
		message = 'The file "' + fn + '" was uploaded successfully to ' + uploadDir
else:
	message = 'No file was uploaded'

replyhtml = """
<!DOCTYPE html>
<html lang="en">

<head>
	<link rel="icon" type="image/x-icon" href="/images/favicon.ico">
	<link rel="stylesheet" type="text/css" href="/pouet.css">
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Document</title>
</head>
<body>
	<h1 style="margin-left: 5em;">Bello Crakito</h1>

	<form action="/cgi-bin/helloworldpy.py" method="POST">
		<label for="fname">First name:</label><br>
		<input type="text" id="fname" name="first_name" value="Mederis"><br>
		<label for="lname">Last name:</label><br>
		<input type="text" id="lname" name="last_name" value="Frambourt"><br><br>
		<input type="submit" value="Submit">
	  </form>

	<img style="margin-left:15ch;" src="/images/crakette.png">
	  <a href="/cgi-bin/helloperl.pl">Tching Tchong</a>

	  <FORM action="http://localhost:8080/cgi-bin/upload.py"
       enctype="multipart/form-data"
       method="post">
	<P>
	What files are you sending? <INPUT type="file" name="filename"><BR>
	<INPUT type="submit" value="Send">
	</FORM>
	<p>%s</p>


</body>
</html>
"""
print(replyhtml % message)