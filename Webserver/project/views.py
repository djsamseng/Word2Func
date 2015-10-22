from flask import render_template, request
from project import app
from backend import search
import json

@app.route('/')
def index():
    urls = "empty"
    return render_template('index.html', urls=urls)

@app.route('/get_links', methods=['POST'])
def get_links():
    return json.dumps(search.get_urls(request.json['query']))
    
