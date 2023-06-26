import mqtt
import db
import weather_api as weather
from flask import Flask
from flask import request
import time

app = Flask(__name__)

mqtt.init()
db.init()
weather.init()

@app.route('/weather')
def get_weather_data():
    return "Hello, World!"

@app.route('/')
def get_data_between_timestamps():
    timestamp1 = request.args.get("timestamp1") or 0

    current_time = round(time.time() * 1000)

    timestamp2 = request.args.get("timestamp2") or current_time
    return db.get_data_between_timestamps(timestamp1, timestamp2)


if __name__ == '__main__':
    app.run()
