from flask import render_template
from project import app
from backend import search

@app.route('/')
def index():
    return render_template('index.html', urls=search.get_urls('test'))
