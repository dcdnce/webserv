import os
import sys
import cgi
import cgitb; cgitb.enable()  # for troubleshooting

params = cgi.FieldStorage()

html = f"""
<!DOCTYPE html>
<html>
<body>
<h1>Test</h1>
<p>Params: {params}</p>
</body>
</html>
"""

print("Content-type: text/html")
print()
print(html)
