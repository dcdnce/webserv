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
print ("<title>pokemon.py</title>")
print ("</head>")
print ("<body>")
print (f"<h1>{data['name'].capitalize()}</h1>")
print (f"<img src=\"{data['sprites']['front_default']}\" />")
print ("</body>")
print ("</html>")
