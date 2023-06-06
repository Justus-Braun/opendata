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
