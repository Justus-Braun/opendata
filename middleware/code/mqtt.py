import sys
import paho.mqtt.client as mqtt
import json
from threading import Thread

import db

User = "opendata-temperature@ttn"
Password = 'NNSXS.77ZDGYBJIDJ65AZU2O6JB5GPKCGHHSQBDRZ2FZQ.XRMPHANORKM5MJQK47N4LESMRG4PN3ENP6G2QA3PKR6OLHKTY3OQ'
theRegion = "EU1"  # The region you are using
port = 8883  # Secure port
run = True


def on_connect(mqttc, obj, flags, rc):
    print("\nConnect: rc = " + str(rc), flush=True)


def on_message(mqttc, obj, msg):
    data = None
    device_id = None
    try:
        print(msg.payload, flush=True)
        parsedJSON = json.loads(msg.payload)
        data = parsedJSON['uplink_message']['decoded_payload']
        device_id = parsedJSON['end_device_ids']['device_id']
    except:
        print("Message has wrong data format (Could be caused by an device joining)", flush=True)
        pass

    if data is not None:
        db.insert_data(device_id, data)


def thread_function(client):
    while run:
        client.loop(10)


def init():
    client = mqtt.Client("", True, None, mqtt.MQTTv31)

    client.on_connect = on_connect
    client.on_message = on_message

    client.username_pw_set(User, Password)

    client.tls_set()

    client.connect(theRegion.lower() + ".cloud.thethings.network", port=port, keepalive=60, bind_address="")

    client.subscribe("#", 0)

    try:
        thread = Thread(target=thread_function, args=(client,))
        thread.start()

    except KeyboardInterrupt:
        print("Exit")
        sys.exit(0)
