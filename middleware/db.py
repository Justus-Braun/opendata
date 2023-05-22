import json

from influxdb import InfluxDBClient

db = 'pyexample'

client = InfluxDBClient(host='localhost', port=8086, username='admin', password='admin')


def init():
    if db not in client.get_list_database():
        client.create_database(db)

    client.switch_database(db)


def insert(data):
    return client.write_points(data)


if __name__ == '__main__':
    init()
    json_body = [
        {
            "measurement": "brushEvents",
            "tags": {
                "user": "Carol",
                "brushId": "6c89f539-71c6-490d-a28d-6c5d84c0ee2f"
            },
            "time": "2018-03-28T8:01:00Z",
            "fields": {
                "duration": 127
            }
        },
        {
            "measurement": "brushEvents",
            "tags": {
                "user": "Carol",
                "brushId": "6c89f539-71c6-490d-a28d-6c5d84c0ee2f"
            },
            "time": "2018-03-29T8:04:00Z",
            "fields": {
                "duration": 132
            }
        },
        {
            "measurement": "brushEvents",
            "tags": {
                "user": "Carol",
                "brushId": "6c89f539-71c6-490d-a28d-6c5d84c0ee2f"
            },
            "time": "2018-03-30T8:02:00Z",
            "fields": {
                "duration": 129
            }
        }
    ]

    result = insert(json_body)
    print(result)
    



