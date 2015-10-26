def is_visible(elem):
    import re
    if elem.parent.name in ['style', 'script', '[document]', 'head', 'title']:
        return False
    elif re.match('<!--.*-->', elem.encode('ascii', 'ignore')):
        return False
    return True

def url_to_file(url, filename):
    import os.path
    import json
    import bs4
    import requests

    data = dict()
    if os.path.isfile(filename):
        with open(filename) as f:
            data = json.loads(f.read())
    r = requests.get(url)

    req_text = ''.join([w if ord(w) < 128 and ord(w) > 31 else ' ' for w in r.text.encode('utf-8')])
    soup = bs4.BeautifulSoup(req_text, 'html.parser')
    text_list = soup.findAll(text=True)
    text = ' '.join(filter(is_visible, text_list))
    text = text.encode('ascii', 'ignore')
    text = text.replace('\n', ' ').replace('\t', ' ')
    text = ' '.join(text.split())
    
    data[url] = text
    with open(filename, 'w') as f:
        f.write(json.dumps(data))

if __name__ == '__main__':
    url_to_file('https://en.wikipedia.org/wiki/Tom_Brady', 'data.json')
    
