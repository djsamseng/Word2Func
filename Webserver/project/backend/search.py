import requests

API_KEY = 'AIzaSyB0jaur8fE5AMTxWHrTYGp--N8BWgd4CFU'
CX_KEY = '016888903529204161084:udpb71g1u8a'
API_URL = 'https://www.googleapis.com/customsearch/v1'

def get_urls(query):
    params = { 'key' : API_KEY, 'cx' : CX_KEY, 'q' : query }
    r = requests.get(API_URL, params = params)
    json = r.json()
    results = []
    for item in json['items']:
        results.append({ 'title':item['title'], 'snippet':item['snippet'], 'link':item['link']})
    return results
