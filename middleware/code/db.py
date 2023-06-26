from influxdb import InfluxDBClient
import time
import os

from dotenv import load_dotenv

load_dotenv()

DB_NAME = os.getenv("DB_NAME") or "measurements"
DB_WEATHER_API = "weather_api"
EVENT_NAME = os.getenv("EVENT_NAME") or 'measurementEvent'

client = InfluxDBClient(host='influxdb', port=8086)


def init():
    try:
        client.drop_database(DB_NAME)
    except:
        pass
        
    try:
        client.drop_database(DB_WEATHER_API)
    except:
        pass

    client.create_database(DB_WEATHER_API)
    client.create_database(DB_NAME)


def convert_data_to_fields(data):
    fields = {}
    for key, value in data.items():
        fields[key] = value
    return fields

def insert_data_weather_api(data): 
    current_time = round(time.time() * 1000)
    json_body = [
        {
            "measurement": "weather_measurements",
            "event": "weatherEvent",
            "tags": {
                "device_id": "open_weather"
            },
            "time": current_time,
            "fields": data
        }
    ]

    client.write_points(json_body, database=DB_WEATHER_API, time_precision='ms', batch_size=10000)


def insert_data(device_id, data, event=EVENT_NAME):
    current_time = round(time.time() * 1000)
    json_body = [
        {
            "measurement": "ttn_measurements",
            "event": event,
            "tags": {
                "device_id": device_id
            },
            "time": current_time,
            "fields": convert_data_to_fields(data)
        }
    ]

    client.write_points(json_body, database=DB_NAME, time_precision='ms', batch_size=10000)

def get_data_between_timestamps(timestamp1, timestamp2):
    query = 'SELECT * FROM "ttn_measurements" WHERE time > {}ms AND time < {}ms'.format(timestamp1, timestamp2)
    query_weather = 'SELECT * FROM "weather_measurements" WHERE time > {}ms AND time < {}ms'.format(timestamp1, timestamp2)
    result = client.query(query, database=DB_NAME)
    result_weather = client.query(query_weather, database=DB_WEATHER_API)
    return list(list(result.get_points()) + list(result_weather.get_points()))
