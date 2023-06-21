# opendata-temperature

# Table of Contents
1. [Installation](#installation)
2. [Nutzung](#nutzung)
3. [API](#api)


## Installation
Board: [CubeCell HTCC-AB01](https://heltec.org/project/htcc-ab01/)
1. Board hinzufügen
    - Arduino IDE (aktuell: Version 2.1.0) -> `File` -> `Preferences...` -> `Additional boards manager URLs`
    - Link einfügen:
    -     https://github.com/HelTecAutomation/CubeCell-Arduino/releases/download/V1.5.0/package_CubeCell_index.json
    - `Boards Manager` öffnen und `CubeCell Development Framework` (aktuell: Version 1.5.0) installieren

2. Temperatur Sensor ([Link](https://wiki.dfrobot.com/SHT31_Temperature_Humidity_Sensor_Weatherproof_SKU_SEN0385))
    - `Library Manager` öffnen und 
        - `Adafruit SHT31 Library` (aktuell: Version 2.2.0) und
        - `Adafruit BusIO` (**Version 1.7.0** !!!) installieren (neuere Versionen funktionieren nicht)
    - Example: `File` -> `Examples` -> `Adafruit SHT31 Library` -> `SHT31test`

3. LoRa
    - ...

## Nutzung

## API

## Middleware
Create an `.env` file in `middleware/` with content
```env
#Grafana
GF_LOG_LEVEL=debug
GF_SECURITY_ADMIN_USER=admin
GF_SECURITY_ADMIN_PASSWORD=admin

#InfluxDB
INFLUXDB_ADMIN_USER=admin
INFLUXDB_ADMIN_PASSWORD=admin
```
Create an `.env` file in `middleware/code/` with content
[openweathermap.org](https://openweathermap.org/)
[latlong.net](https://www.latlong.net/)
```env
#TTN 
TTN_USER=
TTN_PASSWORD=
TTN_REGION=eu1

#Influxdb
DB_NAME=measurements
EVENT_NAME=measurementEvent

#OpenWeather
WEATHER_LATITUDE=
WEATHER_LONGITUDE=
OPEN_WEATHER_API_KEY=
```

