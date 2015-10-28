def is_visible(elem):
    import re
    if elem.parent.name in ['style', 'script', '[document]', 'head', 'title']:
        return False
    elif re.match('<!--.*-->', elem.encode('ascii', 'ignore')):
        return False
    return True

def url_to_file(url, text_file='data.json'):
    import os.path
    import json
    import bs4
    import requests

    data = dict()
    if os.path.isfile(text_file):
        with open(text_file) as f:
            data = json.loads(f.read())
    old_text = ''
    if url in data:
        old_text = data[url]
   
    r = requests.get(url)
    req_text = ''.join([w if ord(w) < 128 and ord(w) > 31 else ' ' for w in r.text.encode('utf-8')])
    soup = bs4.BeautifulSoup(req_text, 'html.parser')
    text_list = soup.findAll(text=True)
    text = ' '.join(filter(is_visible, text_list))
    text = text.encode('ascii', 'ignore')
    text = text.replace('\n', ' ').replace('\t', ' ')
    text = ' '.join(text.split())
   

    data[url] = text
    with open(text_file, 'w') as f:
        f.write(json.dumps(data))

    sig_terms(url, old_text.split(), text.split())

def sig_terms(url, old_words, new_words, sig_terms_file='sig_terms.json'):
    import os.path, json
    file_contents = {'urls':[],'N':0,'word_counts':{}}
    if os.path.isfile(sig_terms_file):
        with open(sig_terms_file) as f:
            file_contents = json.loads(f.read())
    if url in file_contents['urls']:
        remove_sig_terms(old_words, file_contents['word_counts'])
        file_contents['N'] -= len(old_words) 
    else:
        file_contents['urls'].append(url)
    file_contents['word_counts'] = add_sig_terms(new_words, file_contents['word_counts'])
    file_contents['N'] += len(new_words)
    
    with open(sig_terms_file, 'w') as f:
        f.write(json.dumps(file_contents))

def add_sig_terms(new_words, word_counts):
    for word in new_words:
        if word in word_counts:
            word_counts[word] += 1
        else:
            word_counts[word] = 1
    return word_counts

def remove_sig_terms(old_words, word_counts):
    for word in old_words:
        word_counts[word] -= 1
        assert(word_counts[word] >= 0)
    return word_counts
    
if __name__ == '__main__':
    url_to_file('https://en.wikipedia.org/wiki/Tom_Brady', 'data.json')
    
