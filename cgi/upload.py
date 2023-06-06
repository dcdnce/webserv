#!/usr/bin/env python

import cgi
import os

params = cgi.FieldStorage()

ret = "File not uploaded."
if 'filename' in params and os.path.basename(params['filename'].filename) != '':
	fileitem = params['filename']
	fileName = os.path.basename(fileitem.filename)

	uploadDir = os.environ.get('UPLOAD_DIRECTORY')
	if uploadDir == '':
		uploadDir = "./uploads/"
	if uploadDir[-1] != '/':
		uploadDir += '/'
	if os.path.exists(uploadDir) == False:
		os.makedirs(uploadDir)

	open(uploadDir + fileName, "wb").write(fileitem.file.read())
	ret = "File successfully uploaded."


print ("Content-type:text/html\r\n")
print ("<html>")
print ("<head>")
print("<meta charset=\"UTF-8\">")
print ("<title>Upload</title>")
print ("</head>")
print ("<body>")
print ("<h1>Upload</h1>")
print (f"<p>{ret}</p>")
print ("<h1>Debug data</h1>")
print ("<pre>")
print (f"params: {params} <br />")
print ("</pre>")
print ("</body>")
print ("</html>")
