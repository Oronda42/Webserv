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
			uploadDir = './uploads/'

		# Add / at the end if none is present
		if (uploadDir[-1] != '/'):
			uploadDir += '/'

		if not os.path.exists(uploadDir):
			os.makedirs(uploadDir)
		open(uploadDir + fn, 'wb').write(fileitem.file.read())
		message = 'The file "' + fn + '" was uploaded successfully'
else:
	message = 'No file was uploaded'

replyhtml = """
<html>
<body>
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