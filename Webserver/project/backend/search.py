import requests
import api_keys

def get_urls(query):
    params = { 'key' : api_keys.API_KEY, 'cx' : api_keys.CX_KEY, 'q' : query }
    r = requests.get(api_keys.API_URL, params = params)
    json = r.json()
    results = []
    for item in json['items']:
        results.append({ 'id': item['cacheId'], 'title':item['title'], 'snippet':item['snippet'], 'link':item['link']})
    return results
