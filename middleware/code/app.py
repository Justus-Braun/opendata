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

@app.route('/')
def get_data_between_timestamps():
    first_time_point = request.args.get("first_time_point") or 0

    current_time = round(time.time() * 1000)

    latest_time_point = request.args.get("latest_time_point") or current_time
    return db.get_data_between_timestamps(int(first_time_point), int(latest_time_point))


if __name__ == '__main__':
    app.run()
