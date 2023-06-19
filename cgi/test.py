# Imports
import cgi
import requests
import cgitb; cgitb.enable()

# Get the parameters
params = cgi.FieldStorage()

html = f"""
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Test</title>
</head>
<body>
<p>{params}</p>
</body>
</html>
"""

print("Content-Type: text/html")
print()
print(html)
