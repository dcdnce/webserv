#!/usr/bin/env python

import cgi
import requests

params = cgi.FieldStorage()

def fetchData(pokemonName):
    url = f"https://pokeapi.co/api/v2/pokemon/{pokemonName}"
    response = requests.get(url)
    return response.json()

pokemonName = params.getvalue("pokemon") or "pikachu"

data = fetchData(pokemonName)

print ("Content-type:text/html")
print ()
print ("<html>")
print ("<head>")
print ("<meta charset=\"UTF-8\">")
print ("<title>test.py</title>")
print ("</head>")
print ("<body>")
print (f"<h1>{data['name'].capitalize()}</h1>")
print ("<img src=\"{}\" alt=\"{}\">".format(data['sprites']['front_default'], data['name']))
print ("<p>Height: {}</p>".format(data['height']))
print ("<p>Weight: {}</p>".format(data['weight']))
print ("<p>Abilities:</p>")
print ("<ul>")
for ability in data['abilities']:
    print (f"<li>{ability['ability']['name']}</li>")
print ("</ul>")
print ("<p>Types:</p>")
print ("<ul>")
for type in data['types']:
    print (f"<li>{type['type']['name']}</li>")
print ("</ul>")
print ("<p>Stats:</p>")
print ("<ul>")
for stat in data['stats']:
    print (f"<li>{stat['stat']['name']}: {stat['base_stat']}</li>")
print ("</ul>")
print ("<p>Moves:</p>")
print ("<ul>")
for move in data['moves']:
    print (f"<li>{move['move']['name']}</li>")
print ("</ul>")
print ("<p>Forms:</p>")
print ("<ul>")
for form in data['forms']:
    print (f"<li>{form['name']}</li>")
print ("</ul>")
print ("</body>")
print ("</html>")
