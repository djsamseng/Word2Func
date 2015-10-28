from elasticsearch import Elasticsearch

def query(es, phrase, index='wiki', doc_type='articles'):
    body = '''
    {
        "query": {
            "match_phrase": {
                "text": {
                    "query": "%s",
                    "slop": 50
                }
            }
        }
    }''' % (phrase)
    return es.search(index=index, doc_type=doc_type, body=body, _source=False)
