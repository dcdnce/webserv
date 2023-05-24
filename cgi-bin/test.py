#!/usr/bin/env python

import cgi

params = cgi.FieldStorage()

print("Content-type: text/html\r\n\r\n")

print("<html>")
print("<head>")
print("<title>Python Cgi Test</title>")
print("</head>")
print("<body>")
print("<h1>Query String Test</h1>")

# Check if any query string parameters are present
if params:
    print("<p>Query string parameters:</p>")
    print("<ul>")
    for param in params:
        print("<li><strong>{}</strong>: {}</li>".format(param, params[param].value))
    print("</ul>")
else:
    print("<p>No query string parameters found.</p>")

print("</body>")
print("</html>")
