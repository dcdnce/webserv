#!/usr/bin/env python

import cgi
import os

params = cgi.FieldStorage()


if 'filename' not in params or os.path.basename(params['filename'].filename) == '':
	ret = "File not uploaded."
else:
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
print("<script type=\"text/javascript\">")
print("window.location.href = \"/\"")
print(f"alert(\"{ret}\")")
print("</script>")
print ("<title>Upload</title>")
print ("</head>")
print ("</html>")