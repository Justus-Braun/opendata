#!/usr/bin/python3

import sys
import paho.mqtt.client as mqtt
import json
from threading import Thread

import db

User = "opendata-temperature@ttn"
Password = 'NNSXS.77ZDGYBJIDJ65AZU2O6JB5GPKCGHHSQBDRZ2FZQ.XRMPHANORKM5MJQK47N4LESMRG4PN3ENP6G2QA3PKR6OLHKTY3OQ'
theRegion = "EU1"  # The region you are using
run = True


def on_connect(mqttc, obj, flags, rc):
    print("\nConnect: rc = " + str(rc))


def on_message(mqttc, obj, msg):
    parsedJSON = json.loads(msg.payload)
    data = parsedJSON['uplink_message']['decoded_payload']
    db.insert_data(1, data['temperature'], data['battery'], data['humidity'])


def thread_function(mqttc):
    while run:
        mqttc.loop(10)


def mqtt_init():
    mqttc = mqtt.Client()

    mqttc.on_connect = on_connect
    mqttc.on_message = on_message

    mqttc.username_pw_set(User, Password)

    mqttc.tls_set()

    mqttc.connect(theRegion.lower() + ".cloud.thethings.network", 8883, 60)

    mqttc.subscribe("#", 0)

    try:
        thread = Thread(target=thread_function, args=(mqttc,))
        thread.start()

    except KeyboardInterrupt:
        print("Exit")
        sys.exit(0)
