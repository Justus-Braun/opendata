#!/usr/bin/env python3

from influxdb import InfluxDBClient
import time

DB_NAME = 'messurements'
EVENT_NAME = 'temperatureEvents'
MEASUREMENT_NAME = "temperatureMeasurement"

client = InfluxDBClient(host='influxdb', port=8086)


def init():
    client.drop_database(DB_NAME)
    client.create_database(DB_NAME)


def insert_data(id, temperature, battery, humidity):
    current_time = round(time.time() * 1000)
    json_body = [
        {
            "measurement": MEASUREMENT_NAME,
            "event": EVENT_NAME,
            "tags": {
                "id": id
            },
            "time": current_time,
            "fields": {
                "temperature": temperature,
                "battery": battery,
                "humidity": humidity
            }
        }
    ]

    print(current_time)

    client.write_points(json_body, database='writetest', time_precision='ms', batch_size=10000)
