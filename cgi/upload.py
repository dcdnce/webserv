import os
import cgi
import cgitb; cgitb.enable()  # for troubleshooting

redirectTime = 5
params = cgi.FieldStorage()
uploadPath = os.environ.get('UPLOAD_PATH')
uploaded = False
uploadStatus = "File not uploaded."

if 'file' in params:
	fileitem = params['file']

	if (uploadPath[-1] != '/'):
		uploadPath += '/'

	filename = os.path.basename(fileitem.filename)

	if (os.path.exists(uploadPath + filename)):
		uploadStatus = "File already exists."
	else:
		open(uploadPath + filename, 'wb').write(fileitem.file.read())
		uploadStatus = "File uploaded."
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
	flex-direction: column;
}}

section {{
	background-color: #fcc;
	border: 1px solid #c00;
	padding: 1em 2em;
	border-radius: 8px;
}}

section.uploaded {{
	background-color: #cfc;
	border-color: #0c0;
}}

</style>
</head>
<body>
<section class="{"uploaded" if uploaded else ""}">
<h1>{uploadStatus}</h1>
<p>Redirecting in <span id="seconds">{redirectTime}</span> seconds...</p>
</section>
<script>
let seconds = {redirectTime};

setInterval(() => {{
	seconds--;
	document.getElementById('seconds').innerText = seconds;
}}, 1000);

setTimeout(() => {{
	window.location.href = document.referrer;
}}, {redirectTime * 1000})
</script>
</body>
</html>
"""


print ("Content-type:text/html")
print ()
print (html)
