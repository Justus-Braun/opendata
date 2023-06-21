import json
import os
import sched
import time
import requests

from dotenv import load_dotenv

import db

load_dotenv()

WEATHER_LATITUDE = os.getenv('WEATHER_LATITUDE')
WEATHER_LONGITUDE = os.getenv('WEATHER_LONGITUDE')
OPEN_WEATHER_API_KEY = os.getenv('OPEN_WEATHER_API_KEY')
URL = f"https://api.openweathermap.org/data/2.5/weather?lat={WEATHER_LATITUDE}&lon={WEATHER_LONGITUDE}&appid={OPEN_WEATHER_API_KEY}"

INTERVAL = 60 * 10 # 10 minutes

def get_weather_data():
    res = requests.get(URL)
    text = res.text
    parsed_json = json.loads(text)
    data = {
        "temperature": parsed_json['main']['temp'],
        "humidity": parsed_json['main']['humidity'],
    }

    db.insert_data_weather_api(data)


def on_interval(sc):
    get_weather_data()    
    sc.enter(INTERVAL, 1, on_interval, (sc,))


def init():
    get_weather_data()
    s = sched.scheduler(time.time, time.sleep)
    s.enter(INTERVAL, 1, on_interval, (s,))
    s.run()
