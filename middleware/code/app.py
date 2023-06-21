import mqtt
import db
import weather_api as weather
from flask import Flask

app = Flask(__name__)

mqtt.init()
db.init()
weather.init()


@app.route('/')
def hello_world():  # put application's code here
    return 'Hello World!'


if __name__ == '__main__':
    app.run()
