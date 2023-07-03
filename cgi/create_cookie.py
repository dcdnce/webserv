import cgi
import http.cookies
import os

params = cgi.FieldStorage()

username = params.getvalue('username')
cookie = [
	f"Set-Cookie:webservUsername = {username}",
	"Max-Age = 3600",
	#"Domain = localhost", # No domain so it works on all machines (if no domain is provided, the value will be the host of the url of current document)
	"Path = /",
]

print("; ".join(cookie))
print ("Content-type:text/html\r\n")
print ("<html>")
print ("<head>")
print("<meta charset=\"UTF-8\">")
print("<script type=\"text/javascript\">")
print("window.location.href = \"/\"")
print("</script>")
print ("</head>")
print ("</html>")


