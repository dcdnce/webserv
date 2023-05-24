#!/usr/bin/env python

print("Content-type: text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>CGI Test</title>")
print("</head>")
print("<body>")
print("<h1>CGI Test Page</h1>")
for i in range(1, 50):
	print(f"<p>{i}</p>")
print("</body>")
print("</html>")
