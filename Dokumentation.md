# Dokumentation

## Inhaltsverzeichnis
1. [Projektdefintion und -umfeld](#projekt)
2. [Systemarchitektur](#architektur)
3. [Einrichtung](#einrichtung)
4. [Hardware](#hardware)
   1. [Temperatursensor](#sensor)
   2. [Microcontroller](#microcontroller)
   3. [Setup](#setup)
5. [Kommunikation](#kommunikation)
   1. [LoraWan](#lorawan)
   2. [MQTT](#mqtt)
6. [Middleware](#middleware)
7. [Datenbank](#datenbank)
8. [Visualisierung](#visualisierung)
9. [Anwendung](#anwendung)

## Projektdefintion und -umfeld

Im Rahmen des Moduls "Programmierung 2" für den Studiengang Informatik (B. Sc.) an der Hochschule Bonn-Rhein-Sieg haben wir einen Temperatur-Sensor mit Web-Visualisierung implementiert.
Das Projekt hatte einen Zeitraum von ungefähr 7 Wochen und wurde neben dem regulären Studium realisiert. 
Für das Projekt wurde uns die Hardware (Sensor, Microcontroller, Batterien, Solarzelle LINKSSSSSS) zur Verfügung gestellt.

## Systemarchitektur

Die Systemarchitektur unseres Projekts umfasst mehrere Komponenten, die gemeinsam die Erfassung und Darstellung der Temperaturdaten ermöglichen.

Der Temperatursensor misst die Temperatur und ist über eine Schnittstelle mit einem Microcontroller verbunden. 
Der Microcontroller fungiert als Bindeglied zwischen dem Sensor und dem LoRaWAN-Netzwerk. 
Er verarbeitet die Sensorwerte und erstellt Datenpakete, die über das LoRaWAN-Protokoll übertragen werden.

Die LoRaWAN-Gateways empfangen die Datenpakete von den Microcontrollern und leiten sie an das The Things Network (TTN) weiter. 
TTN ist eine LoRaWAN-Infrastruktur, die als zentrale Datenvermittlungsstelle fungiert. 
Dort werden die Daten gesammelt und über das MQTT-Protokoll veröffentlicht.

Die Python-Middleware fungiert als MQTT-Client und empfängt die veröffentlichten Daten über MQTT. 
Sie übernimmt die Verarbeitung der empfangenen Daten, einschließlich Transformationen und Validierung, und leitet sie an die InfluxDB weiter.

Die InfluxDB ist eine Zeitreihendatenbank, in der die Temperaturdaten gespeichert werden. 
Sie bietet eine effiziente Speicherung und Abfrage von zeitabhängigen Daten und ermöglicht es uns, die gesammelten Daten einfach zu verwalten und zu analysieren.

Für die Darstellung und Visualisierung der Daten nutzen wir Grafana. Grafana ist ein Open-Source-Tool zur Erstellung von Dashboards und Diagrammen. 
Es greift auf die InfluxDB zu, um die gespeicherten Temperaturdaten abzurufen und sie in ansprechender und interaktiver Form darzustellen. 
Grafana bietet uns die Möglichkeit, benutzerdefinierte Dashboards zu erstellen und die Temperaturdaten in Echtzeit zu überwachen.

Diese Systemarchitektur ermöglicht eine nahtlose Erfassung, Übertragung, Verarbeitung, Speicherung und Darstellung der Temperaturdaten. 
Jede Komponente erfüllt eine spezifische Rolle und arbeitet zusammen, um ein zuverlässiges und effizientes System für die Temperaturüberwachung bereitzustellen. 


## Einrichtung

#### Erstellen einer `.env`-Datei im Verzeichnis `middleware/` mit folgendem Inhalt:

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

Für die Attribute "TTN_USER" und "TTN_PASSWORD" sind Login-Daten einzutragen.
Die Werte für "WEATHER_LATITUDE" und "WEATHER_LONGITUDE" sind [latlong.net](https://www.latlong.net/) zu entnehmen.
Der Wert für "OPEN_WEATHER_API_KEY" ist in [openweathermap.org](https://openweathermap.org/) zu erstellen.


## Hardware
### Temperatursensor ([SKU:SEN0385](https://wiki.dfrobot.com/SHT31_Temperature_Humidity_Sensor_Weatherproof_SKU_SEN0385))

Eine zentrale Komponente unseres Systems ist der Sensor SKU:SEN0385.
Dieser Sensor ist in der Lage, sowohl die Temperatur in Grad Celsius als auch die Luftfeuchtigkeit in Prozent präzise und zuverlässig zu messen.
Der Sensor arbeitet effizient und zeichnet sich durch eine hohe Genauigkeit und Stabilität aus.


### Microcontroller ([Heltec CubeCell HTCC-AB01](https://heltec.org/project/htcc-ab01/))

Im Rahmen unseres Projekts ist der Sensor SKU:SEN0385 mit dem Microcontroller Heltec CubeCell HTCC-AB01 verbunden, der die Aufgabe hat, die Messwerte des Sensors zu erfassen und sie für die Übertragung über das LoRaWAN-Netzwerk vorzubereiten.
Als speziell für den Einsatz von IoT-Anwendungen entwickelter Microcontroller, verfügt er über eine integrierte LoRaWAN-Konnektivität und ist in der Lage, Daten drahtlos über große Entfernungen zu übertragen.
Die vom Sensor erfassten Daten werden vom Microcontroller in XXX Abständen ausgelesen und an die LoRaWAN-Gateways gesendet.


### Setup

#### Voraussetzung
Arduino IDE (Version 2.1.0)

#### Anleitung
1. Board installieren
   -`File` -> `Preferences...` -> `Additional boards manager URLs` -> Link einfügen:
   https://github.com/HelTecAutomation/CubeCell-Arduino/releases/download/V1.5.0/package_CubeCell_index.json
   -`Boards Manager` öffnen und `CubeCell Development Framework` (aktuell: Version 1.5.0) installieren
3. Library installieren
   -`Library Manager` öffnen -> `Adafruit SHT31 Library` (aktuell: Version 2.2.0) und `Adafruit BusIO` (**Version 1.7.0** !!!) installieren (neuere Versionen funktionieren nicht)
    - Beispiel: `File` -> `Examples` -> `Adafruit SHT31 Library` -> `SHT31test` 

## Kommunikation
Unser Projekt nutzt LoRaWAN, ein energiesparendes drahtloses Netzwerkprotokoll, um die Temperaturdaten von dem Sensor an das TTN zu übertragen.

Die Datei lora-temperature.ino implementiert die Hauptlogik für die Kommunikation des Microcontrollers mit dem LoRaWAN-Netzwerk und dem Sensor SKU:SEN0385. 
Der Code verwendet die LoRaWAN-Bibliothek zur Kommunikation über das LoRaWAN-Protokoll.

In der loop()-Funktion wird der Zustand des Geräts überwacht und entsprechende Aktionen ausgeführt. Der Code wechselt zwischen den verschiedenen Gerätezuständen, wie z.B. Initialisierung, Beitritt zum Netzwerk, Senden der Daten und Ruhezustand. Je nach Zustand werden die erforderlichen Aktionen ausgeführt, wie z.B. das Senden der Datenpakete über LoRaWAN oder das Planen des nächsten Sendevorgangs.

Zum Auslesen der Daten aus dem Sensor wird die Funktion readSensor() ausgeführt. Hierbei werden Werte für die Batteriespannung, Luftfeuchtigkeit und Temperatur erfasst. Die Funktion überprüft auch, ob sich der Sensor im aktiven Modus befindet und aktualisiert die Daten nur dann, wenn sich die Werte geändert haben.


## Middleware

Die Middleware-Komponente des Temperatursensor-Projekts umfasst eine Reihe von python-Dateien, die für die Kommunikation mit externen Diensten, die Datenverarbeitung und die Datenbankzugriffe verantwortlich sind.
Aufgrund der Einfachheit und großen Anzahl von nützlichen Bibliotheken haben wir uns für python entschieden.

Im Folgenden werden die einzelnen Dateien und ihre Funktionen beschrieben:

1. app.py

Diese Datei ist der Hauptpunkt der Anwendung und verwendet das Flask-Framework, um HTTP-Anfragen zu verarbeiten. Derzeit ist folgender Endpunkt definiert:

/: Abruf von Daten aus der Datenbank. Über Parameter können Zeitstempel angegeben werden, um Daten zwischen zwei Zeitpunkten abzurufen. Standardmäßig werden der Zeitpunkt "0" (Anfang) und der aktuelle Zeitpunkt verwendet.


2. mqtt.py

Die mqtt.py-Datei stellt die MQTT-Kommunikation mit dem "The Things Network" (TTN) her, um Messdaten von den angeschlossenen Geräten zu empfangen. Die wichtigsten Funktionen sind:

on_connect(mqttc, obj, flags, rc): Diese Funktion wird aufgerufen, wenn die Verbindung zum TTN-MQTT-Broker hergestellt wurde.
on_message(mqttc, obj, msg): Diese Funktion wird aufgerufen, wenn eine MQTT-Nachricht empfangen wird. Sie extrahiert die relevanten Daten aus der Nachricht und ruft die 
thread_function(client): Diese Funktion wird in einem separaten Thread ausgeführt und ruft die client.loop()-Funktion auf, um die MQTT-Kommunikation aufrechtzuerhalten.
init(): Diese Funktion initialisiert die MQTT-Kommunikation. Sie erstellt einen MQTT-Client, setzt Benutzername und Passwort, stellt eine TLS-Verbindung zum Broker her, abonniert alle verfügbaren Topics und startet den Hintergrundthread, der die MQTT-Kommunikation aufrechterhält.


3. db.py

Die db.py-Datei enthält Funktionen zur Datenbankverwaltung und -abfrage. Diese Datei enthält Funktionen zur Verbindung mit der InfluxDB-Datenbank. Sie stellt Methoden bereit, um Daten in die Datenbank einzufügen und Daten zwischen zwei Zeitstempeln abzurufen. In dieser Datei wird auch das Gerüst für die Open-Weather-API initialisiert. Die wichtigsten Funktionen umfassen:

init(): Diese Funktion initialisiert die Datenbankverbindung und erstellt die erforderlichen Datenbanken, falls sie nicht bereits vorhanden sind. Es werden zwei Datenbanken initialisiert: measurements für Gerätemessungen und weather_api für Wetterdaten von Open Weather.
convert_data_to_fields(data): Diese Funktion schreibt die empfangenen Daten in ein Array für die Datenbank.
insert_data_weather_api(data): Diese Funktion schreibt die Open-Weather Daten in eine .json fügt sie dann in die Datenbank weather_api ein.
insert_data(device_id, data, event=EVENT_NAME): Diese Funktion schreibt die Gerätemessungen in eine .json fügt sie dann in die Datenbank measurements ein.
get_data_between_timestamps(first_time_point, latest_time_point): Diese Funktion ruft Daten aus der Datenbank measurements zwischen den angegebenen Zeitpunkten ab.


4. weather_api.py

Diese Datei befasst sich mit der Verbindung zur Open-Weather-API und Importierung der Daten.
get_weather_data(): Diese Funktion ruft Wetterdaten von der OpenWeatherMap-API ab. Sie verwendet die angegebenen Koordinaten, den API-Schlüssel und die URL, um eine Anfrage an die API zu senden. Die empfangenen Daten werden verarbeitet und mithilfe der insert_data_weather_api()-Funktion in die separate "weather_api"-Datenbank eingefügt.
on_interval(sc): Diese Funktion wird periodisch in einem festgelegten Intervall aufgerufen und ruft die get_weather_data()-Funktion auf, um aktuelle Wetterdaten abzurufen und einzufügen.
thread_function(): Diese Funktion wird als Hintergrundthread gestartet und ruft die on_interval()-Funktion in regelmäßigen Abständen auf, um Wetterdaten abzurufen.
init(): Diese Funktion initialisiert die Wetter-API-Integration. Sie startet den Hintergrundthread, der die Wetterdaten periodisch abruft und in die Datenbank einfügt.



## Datenbank

### Datenbank-API

Die Datenbank mit den gemessenen Werten verfügt über eine API, die auf dem Endpunkt `http://<IP-Adresse>:5000` verfügbar ist. Durch Aufrufen des Endpunkts `/` können Daten abgerufen werden. Optional können bestimmte Datenbereiche mithilfe von Unix-Zeitstempeln als Abfrageparameter angegeben werden. Diese Parameter werden als `first_time_point` und `latest_time_point` bezeichnet.
- `first_time_point` (optional): Der Unix-Zeitstempel des frühesten Zeitpunkts, ab dem Daten abgerufen werden sollen. Wenn dieser Parameter nicht angegeben wird, werden alle verfügbaren Daten zurückgegeben.
- `latest_time_point` (optional): Der Unix-Zeitstempel des spätesten Zeitpunkts, bis zu dem Daten abgerufen werden sollen. Wenn dieser Parameter nicht angegeben wird, werden alle verfügbaren Daten bis zum aktuellen Zeitpunkt zurückgegeben.

Die Antwort erfolgt im JSON-Format und enthält alle relevanten Informationen zu den abgerufenen Datensätzen.

#### Beispielanfragen
1. Abrufen aller Daten:
```
GET http://<IP-Adresse>:5000
```

2. Abrufen eines bestimmten Datenbereichs:
```
GET http://<IP-Adresse>:5000?first_time_point=1624704000&latest_time_point=1625097600
```
Hierbei wurden als `first_time_point` der Unix-Zeitstempel des 26. Juni 2021 00:00:00 und als `latest_time_point` der Unix-Zeitstempel des 30. Juni 2021 00:00:00 verwendet.


## Visualisierung
Zur Visualisierung der Daten und Interaktion mit dem Anwender wird Grafana verwendet. Grafana ist eine Open-Source-Plattform, die es Benutzern ermöglicht, Daten aus verschiedenen Datenquellen abzurufen, zu analysieren und in ansprechenden Dashboards darzustellen. Grafana unterstützt eine Vielzahl von Datenquellen wie InfluxDB, Prometheus, Elasticsearch, MySQL und viele andere.

### Setup

Um Grafana für die Visualisierung zu nutzen, müssen erst die beiden Datanbanken als Datasource hinzugefügt werden.
Zuerst werden die Open Weather Daten eingefügt:

1. Links im Abschnitt "Data sources" auf "Add data source" klicken
![image](https://github.com/Justus-Braun/opendata/assets/57273189/48f3b115-aee4-4356-874f-0b25a5d12cea)

2. "InfluxDB" auswählen
![image](https://github.com/Justus-Braun/opendata/assets/57273189/fbc1a0bf-8d0f-40c3-9234-7712d538f69d)

3. In den Settings die URL `http://influxdb:8086` angeben
![image](https://github.com/Justus-Braun/opendata/assets/57273189/9793dbf2-cc0c-44b2-90dc-6298ec078e33)

4. In den Details `weather_api` im Feld "Database" eintragen 
![image](https://github.com/Justus-Braun/opendata/assets/57273189/33a7ec5e-bb4b-4935-a0c6-2002df04dac2)

5. Mit "Save & Test" speichern

6. Alle Schritte wiederholen und diesmal `measurements` als Database eintragen

Da nun alle Datasources eingerichtet sind, können die Dashboards hinzugefügt werden.
Im repo ist bereits ein Beispiel-Dashboard hinterlegt: `grafana/example_dashboard.json`

## Anwendung



## AB HIER MÜLL
## AB HIER MÜLL
## AB HIER MÜLL




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



