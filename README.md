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

## API-Dokumentation - Datenbankabfragen

### Einführung
Die vorliegende Dokumentation beschreibt eine API, die auf dem Endpunkt `http://<IP-Adresse>:5000` verfügbar ist. Durch Aufrufen des Endpunkts `/` können Daten aus einer Datenbank abgerufen werden. Optional können bestimmte Datenbereiche mithilfe von Unix-Zeitstempeln als Abfrageparameter angegeben werden. Diese Parameter werden als `first_time_point` und `latest_time_point` bezeichnet.

### Endpunkt
```
GET http://<IP-Adresse>:5000
```

### Parameter
- `first_time_point` (optional): Der Unix-Zeitstempel des frühesten Zeitpunkts, ab dem Daten abgerufen werden sollen. Wenn dieser Parameter nicht angegeben wird, werden alle verfügbaren Daten zurückgegeben.
- `latest_time_point` (optional): Der Unix-Zeitstempel des spätesten Zeitpunkts, bis zu dem Daten abgerufen werden sollen. Wenn dieser Parameter nicht angegeben wird, werden alle verfügbaren Daten bis zum aktuellen Zeitpunkt zurückgegeben.

### Antwort
Die API gibt die abgefragten Daten aus der Datenbank zurück. Die Antwort erfolgt im JSON-Format und enthält alle relevanten Informationen zu den abgerufenen Datensätzen.

### Beispielanfragen
1. Abrufen aller Daten:
```
GET http://<IP-Adresse>:5000
```

2. Abrufen eines bestimmten Datenbereichs:
```
GET http://<IP-Adresse>:5000?first_time_point=1624704000&latest_time_point=1625097600
```
Hierbei wurden als `first_time_point` der Unix-Zeitstempel des 26. Juni 2021 00:00:00 und als `latest_time_point` der Unix-Zeitstempel des 30. Juni 2021 00:00:00 verwendet.

### Hinweise
- Der Endpunkt und die IP-Adresse sollten entsprechend der Implementierung angepasst werden.
- Es wird empfohlen, die Unix-Zeitstempel in UTC zu verwenden, um einheitliche Ergebnisse zu erhalten.
- Die genaue Struktur der zurückgegebenen Daten hängt von der Implementierung der API und der Datenbank ab.

Bitte beachten Sie, dass dies nur eine Beispiel-Dokumentation ist und je nach spezifischer Implementierung weitere Informationen und Anpassungen erforderlich sein können.

## Setup
#### Erstellen einer `.env`-Datei im Verzeichnis `middleware/` mit folgendem Inhalt:

[openweathermap.org](https://openweathermap.org/)

[latlong.net](https://www.latlong.net/)

```env
# Grafana
GF_LOG_LEVEL=debug
GF_SECURITY_ADMIN_USER=admin
GF_SECURITY_ADMIN_PASSWORD=admin

# InfluxDB
INFLUXDB_ADMIN_USER=admin
INFLUXDB_ADMIN_PASSWORD=admin
```

#### Erstellen einer `.env`-Datei im Verzeichnis `middleware/code/` mit folgendem Inhalt:
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

## Grafana einrichten
Zuerst müssen wir unsere Datenbank als Datasource hinzufügen.
Dazu navigieren wir links zu Datasource.

Dort auf Add new Datasource klicken
![image](https://github.com/Justus-Braun/opendata/assets/57273189/48f3b115-aee4-4356-874f-0b25a5d12cea)

Dort Klicken wir auf Influxdb
![image](https://github.com/Justus-Braun/opendata/assets/57273189/fbc1a0bf-8d0f-40c3-9234-7712d538f69d)

In den Settings nutzten wir als URL `http://influxdb:8086`.
![image](https://github.com/Justus-Braun/opendata/assets/57273189/9793dbf2-cc0c-44b2-90dc-6298ec078e33)

Weiter unten fügen wir dann `weather_api` als Datenbank ein. 
![image](https://github.com/Justus-Braun/opendata/assets/57273189/33a7ec5e-bb4b-4935-a0c6-2002df04dac2)

Nun auf Save & Test.

Jetzt wieder holen wir alle schritte noch einmal nur das wir `measurements` als Datenbank nutzen

Da nun alle Datasources eingerichtet sind können wir die Dashboards hinzufügen.
Da kann man sich frei austoben es ist aber auch ein im repo hinterlegt: `grafana/example_dashboard.json`


