#!/usr/bin/env python3
import json

from influxdb import InfluxDBClient
import uuid
import random
import time

client = InfluxDBClient(host='localhost', port=8086)
client.drop_database('writetest')
client.create_database('writetest')

measurement_name = 'm1'
number_of_points = 100
data_end_time = int(time.time() * 1000) #milliseconds


json_body = [
    {
        "measurement": "brushEvents",
        "tags": {
            "user": "Carol",
            "brushId": "6c89f539-71c6-490d-a28d-6c5d84c0ee2f"
        },
        "time": "2023-03-28T8:01:00Z",
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
        "time": "2023-03-29T8:04:00Z",
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
        "time": "2023-03-30T8:02:00Z",
        "fields": {
            "duration": 129
        }
    }
]


client.write_points(json_body, database='writetest', time_precision='ms', batch_size=10000)
