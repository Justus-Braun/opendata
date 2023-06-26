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
    if DB_NAME not in client.get_list_database():
        client.create_database(DB_NAME)
    
    if DB_WEATHER_API not in client.get_list_database():
        client.create_database(DB_WEATHER_API)


def convert_data_to_fields(data):
    fields = {}
    for key, value in data.items():
        if value == "NaN":
            print("NaN value found, skipping. Could be an Error please check the sensor.", flush=True)
            continue

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


def get_data_between_timestamps(first_time_point, latest_time_point):
    if not type(first_time_point) is int:
        pass
    if not type(latest_time_point) is int:
        pass

    query = f"SELECT * FROM ttn_measurements WHERE time >= {first_time_point}ms AND time <= {latest_time_point}ms"
    result = client.query(query, database=DB_NAME)
    return list(result.get_points())
