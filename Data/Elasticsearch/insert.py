from elasticsearch import Elasticsearch, helpers

def bulk_insert_data_json(es, json_file):
   
    data_gen = data_generator(json_file)
    helpers.bulk(es, data_gen)

def data_generator(json_file):
    import json

    with open(json_file) as f:
        data = json.loads(f.read())
        for url in data:
            obj = {
                '_op_type': 'create',
                '_index': 'wiki',
                '_type': 'articles',
                '_id': url,
                'url': url,
                'text': data[url]
            }
            yield obj


