from elasticsearch import Elasticsearch

def get_es():
    es = Elasticsearch()
    return es

def create_index(es, index='wiki', doc_type='articles'):
    body = """
    {
        "settings":{
            "number_of_shards": 5,
            "number_of_replicas": 0
        },
        "mappings": {
            "articles": {
                "properties": {
                    "url": {
                        "type": "string",
                        "index": "not_analyzed"
                    },
                    "text": {
                        "type": "string",
                        "index": "analyzed"
                    }
                }
            }
        }
    }
    """
    return es.indices.create(index=index, body=body)
