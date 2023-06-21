import sys
import paho.mqtt.client as mqtt
import json
import db
import os

from threading import Thread
from dotenv import load_dotenv

load_dotenv()

TTN_USER = os.getenv("TTN_USER") or ""
TTN_PASSWORD = os.getenv("TTN_PASSWORD") 
TTN_REGION = os.getenv("TTN_REGION") or "eu1"  # The region you are using
port = 8883  # Secure port


def on_connect(mqttc, obj, flags, rc):
    print("\nConnect: rc = " + str(rc), flush=True)


def on_message(mqttc, obj, msg):
    data = None
    device_id = None
    try:
        parsed_json = json.loads(msg.payload)
        data = parsed_json['uplink_message']['decoded_payload']
        device_id = parsed_json['end_device_ids']['device_id']
    except:
        print("Message has wrong data format (Could be caused by an device joining)", flush=True)
        pass

    if data is not None:
        db.insert_data(device_id, data)


def thread_function(client):
    while True:
        client.loop(10)


def init():
    client = mqtt.Client("", True, None, mqtt.MQTTv31)

    client.on_connect = on_connect
    client.on_message = on_message

    client.username_pw_set(TTN_USER, TTN_PASSWORD)

    client.tls_set()

    client.connect(TTN_REGION.lower() + ".cloud.thethings.network", port=port, keepalive=60, bind_address="")

    client.subscribe("#", 0)

    try:
        thread = Thread(target=thread_function, args=(client,))
        thread.start()

    except KeyboardInterrupt:
        print("Exit")
        sys.exit(0)
