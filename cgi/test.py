# Imports
import cgi
import requests

# Get the parameters
params = cgi.FieldStorage()

# Functions
def getRandomWord():
	url = "https://random-word-api.vercel.app/api?words=1"
	response = requests.get(url)
	return response.json()[0]

def getDefinition(word):
	url = f"https://api.dictionaryapi.dev/api/v2/entries/en/{word}"
	response = requests.get(url)
	return response.json()[0]['meanings'][0]['definitions'][0]['definition']

word = getRandomWord()
html = f"""
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Test</title>
<style>

* {{
	padding: 0;
	margin: 0;
	box-sizing: border-box;
}}

body {{
	background: #fefefe;
	color: #333;
	font-family: sans-serif;

	display: flex;
	flex-direction: column;
	justify-content: center;
	align-items: center;

	min-height: 100vh;
}}

.container {{
	display: flex;
	flex-direction: column;
	gap: 1rem;

	padding: 1.5rem 2rem;
	background: rgba(0, 0, 0, 0.05);
	border-radius: 0.5rem;
}}

</style>
</head>
<body>
<div class="container">
	<h1>{word.capitalize()}</h1>
	<p>{getDefinition(word)}</p>
</div>
</body>
</html>
"""

# Print the HTML
print("Content-type:text/html")
print()
print(html)
