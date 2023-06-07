#!/usr/bin/env python

import cgi
import os
import cgitb; cgitb.enable() # for troubleshooting

params = cgi.FieldStorage()
uploadPath = os.environ.get('UPLOAD_PATH')
uploaded = False
uploadStatus = "File not uploaded."

if 'filename' in params:
	fileItem = params['filename']

	if not os.path.exists(uploadPath):
		os.makedirs(uploadPath)

	if (uploadPath[-1] != "/"):
		uploadPath += "/"

	fn = os.path.basename(fileItem.filename)
	open(uploadPath + fn, 'wb').write(fileItem.file.read())

	uploadStatus = "File uploaded to: " + uploadPath
	uploaded = True


html = f"""
<html>
<head>
<meta charset="UTF-8">
<title>Upload</title>
<style>
body {{
	font-family: sans-serif;
	height: 100vh;
	margin: 0;
	display: flex;
	align-items: center;
	justify-content: center;
}}

section {{
	border: 1px solid #ccc;
	padding: 20px 40px;
	border-radius: 5px;
}}

section h1::before {{
	content: "❌";
	display: inline-block;
	margin-right: 10px;
}}

section.uploaded h1::before {{
	content: "✅";
}}
</style>
</head>
<body>
<section class="{"uploaded" if uploaded else ""}">
<h1>Upload</h1>
<p>{uploadStatus}</p>
</section>
<script>
setTimeout(function() {{
	window.location.href = "..";
}}, 3000);
</body>
</html>
"""


print ("Content-type:text/html\r\n\r\n")
print (html)
