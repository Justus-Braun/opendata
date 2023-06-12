from flask import Flask
import mqtt
import db

app = Flask(__name__)


@app.route('/')
def hello_world():  # put application's code here
    return 'Hello World!'


if __name__ == '__main__':
    mqtt.mqtt_init()
    db.init()
    app.run()
