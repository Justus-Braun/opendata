from influxdb import InfluxDBClient
import time

DB_NAME = 'measurements'
EVENT_NAME = 'temperatureEvents'
MEASUREMENT_NAME = "temperatureMeasurement"

client = InfluxDBClient(host='influxdb', port=8086)


def init():
    try:
        client.drop_database(DB_NAME)
    except:
        pass
        
    client.create_database(DB_NAME)

def convert_data_to_fields(data):
    fields = {}
    for key, value in data.items():
        fields[key] = value
    return fields


def insert_data(device_id, data):
    current_time = round(time.time() * 1000)
    json_body = [
        {
            "measurement": MEASUREMENT_NAME,
            "event": EVENT_NAME,
            "tags": {
                "device_id": device_id
            },
            "time": current_time,
            "fields": convert_data_to_fields(data)
        }
    ]

    client.write_points(json_body, database=DB_NAME, time_precision='ms', batch_size=10000)
