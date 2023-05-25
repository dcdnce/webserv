#!/usr/bin/env python

import cgi
import os

contentLength = os.environ.get('CONTENT_LENGTH')

params = cgi.FieldStorage()

print("Content-type: text/html")
print("")
print("<html>")
print("<head>")
print("<title>Python Cgi Test</title>")
print("</head>")
print("<body>")
print("<vh1>Python Cgi Test</h1>")

print("<p>Content-Length: {}</p>".format(contentLength))

#Check if any query string parameters are present
if params:
    print("<p>GET or POST parameters:</p>")
    print("<ul>")
    for param in params:
        print("<li><strong>{}</strong>: {}</li>".format(param, params[param].value))
    print("</ul>")
else:
    print("<p>No query string parameters found.</p>")

print("</body>")
print("</html>")
