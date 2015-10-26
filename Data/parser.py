import requests

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

    data = []
    if os.path.isfile(filename):
        with open(filename) as f:
            data = json.loads(f.read())
    r = requests.get(url)

    req_text = ''.join([w if ord(w) < 128 and ord(w) > 31 else ' ' for w in r.text.encode('utf-8')])
    soup = bs4.BeautifulSoup(req_text)
    text_list = soup.findAll(text=True)
    text = ''.join(filter(is_visible, text_list)).encode('ascii', 'ignore').replace('\n', ' ').replace('\t', ' ')

    
    data.append({'url':url, 'text':text})
    with open(filename, 'w') as f:
        f.write(json.dumps(data))
