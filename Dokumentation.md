# OpenData Projekt Temperatur- und Luftfeuchtigkeitssensor und Wasserpegelsensor
**von Oliver Glaser, Justus Braun, Leon Schirra**
<br>
<br>
# Projekt- und Benutzer-Dokumentation

## Inhaltsverzeichnis
- [Projektdefintion und -umfeld](#projekt)
- [Systemarchitektur](#architektur)
- [Einrichtung](#einrichtung)
- [Hardware](#hardware)
   1. [Temperatursensor](#sensor)
   2. [Microcontroller](#microcontroller)
   3. [Setup](#setup)
- [Kommunikation](#kommunikation)
   1. [LoraWan](#lorawan)
   2. [MQTT](#mqtt)
- [Middleware](#middleware)
- [Datenbank](#datenbank)
- [Visualisierung](#visualisierung)


## Projektdefintion und -umfeld

Im Rahmen des Moduls "Programmierung 2" für den Studiengang Informatik (B. Sc.) an der Hochschule Bonn-Rhein-Sieg haben wir 2 Projekte mit Webvisualisierung realisiert: einen Temperatur- und Luffeuchtigkeitssensor und einen Wasserpegelsensor.
Die Projekte hatten einen Zeitraum von 7 Wochen und wurden neben dem regulären Studium realisiert.
Für die Projekte wurde uns die Hardware (Sensoren, Microcontroller, Batterien, Solarzellen, Gehäuse) zur Verfügung gestellt.


## Systemarchitektur

Die Systemarchitektur unserer Projekte umfasst mehrere Komponenten, die gemeinsam die Erfassung und Darstellung der Sensordaten ermöglichen.

Ein Klimasensor misst Temperatur und Luftfeuchtigkeit und ein Abstandssensor misst den Wasserpegel. Beide sind über eine Schnittstelle mit Microcontrollern verbunden.
Die Microcontroller fungieren als Bindeglied zwischen Sensor und LoRaWAN-Netzwerk. 
Sie verarbeiten Sensorwerte und erstellen Datenpakete, die über das LoRaWAN-Protokoll übertragen werden.

Die LoRaWAN-Gateways empfangen die Datenpakete von den Microcontrollern und leiten sie an das The Things Network (TTN) weiter. 
TTN ist eine LoRaWAN-Infrastruktur, die als zentrale Datenvermittlungsstelle fungiert. 
Dort werden die Daten gesammelt und über das MQTT-Protokoll versendet.

Die Python-Middleware fungiert als MQTT-Client und empfängt die versendeten Daten über MQTT. 
Sie übernimmt die Verarbeitung der empfangenen Daten, einschließlich Transformationen und Validierung, und leitet sie an die InfluxDB Datenbank weiter.

Die InfluxDB ist eine Zeitreihendatenbank, in der die Daten gespeichert werden. 
Sie bietet eine effiziente Speicherung und Abfrage von zeitabhängigen Daten und ermöglicht es uns, die gesammelten Daten einfach zu verwalten und zu analysieren.

Für die Visualisierung der Daten nutzen wir Grafana. Grafana ist ein Open-Source-Tool zur Erstellung von Dashboards und Diagrammen im Webbrowser. 
Es greift auf die InfluxDB zu, um die gespeicherten Daten abzurufen und sie in übersichtlicher und interaktiver Form darzustellen. 
Grafana bietet uns die Möglichkeit, benutzerdefinierte Dashboards zu erstellen und die Daten in Echtzeit zu überwachen.

Diese Systemarchitektur ermöglicht eine nahtlose Erfassung, Übertragung, Verarbeitung, Speicherung und Darstellung der Daten. 
Jede Komponente erfüllt eine spezifische Rolle und arbeitet zusammen, um ein zuverlässiges und effizientes System für die Datenüberwachung bereitzustellen. 


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

Für "TTN_USER" ist der MQTT Username und für "TTN_PASSWORD" der API Key von der TTN Application einzutragen.<br>
Die Werte für "WEATHER_LATITUDE" und "WEATHER_LONGITUDE" sind auf [latlong.net](https://www.latlong.net/) für den gewünschten Vergleich-Standort zu entnehmen.<br>
Der Wert für "OPEN_WEATHER_API_KEY" ist in [openweathermap.org](https://openweathermap.org/) zu erstellen.<br>

Um die für dieses Projekte benötigten Services zu starten, wird der Command 
```
docker compose up
```
im Verzeichnis `/middleware` ausgeführt.

Um diese wieder zu stoppen, wird der Command 
```
docker compose down
```
im Verzeichnis `/middleware` ausgeführt.


## Hardware
### Klimasensor ([SKU:SEN0385](https://wiki.dfrobot.com/SHT31_Temperature_Humidity_Sensor_Weatherproof_SKU_SEN0385))

Eine zentrale Komponente unseres Systems ist der Sensor SKU:SEN0385.
Dieser Sensor ist in der Lage, sowohl die Temperatur in Kelvin als auch die Luftfeuchtigkeit in Prozent präzise und zuverlässig zu messen.
Der Sensor arbeitet effizient und zeichnet sich durch eine hohe Genauigkeit und Stabilität aus.

### Abstandssensor ([A01NYUB](https://wiki.dfrobot.com/A01NYUB%20Waterproof%20Ultrasonic%20Sensor%20SKU:%20SEN0313))
Für das zweite Projekt wurde der Sensor A01NYUB verwendet.
Dieser Sensor basiert auf Ultraschalltechnologie und ermöglicht die präzise Messung des Abstands zu einer Oberfläche, in diesem Fall dem Wasserspiegel. Durch die Auswertung der Laufzeit des Ultraschallimpulses können wir den Wasserpegel bestimmen. Der A01NYUB Sensor zeichnet sich durch seine hohe Genauigkeit, Zuverlässigkeit und einfache Handhabung aus, wodurch er ideal für die Anwendung als Wasserpegelsensor geeignet ist.

### Microcontroller ([Heltec CubeCell HTCC-AB01](https://heltec.org/project/htcc-ab01/))

Im Rahmen unseres Projekts wurden die Sensoren mit dem Microcontroller Heltec CubeCell HTCC-AB01 verbunden, der die Aufgabe hat, die Messwerte des angeschlossenen Sensors zu erfassen und sie für die Übertragung über das LoRaWAN-Netzwerk vorzubereiten.
Als speziell für den Einsatz von IoT-Anwendungen entwickelter Microcontroller, verfügt er über eine integrierte LoRaWAN-Konnektivität und ist in der Lage, Daten drahtlos über große Entfernungen zu übertragen.
Die vom Sensor erfassten Daten werden vom Microcontroller in festgelegten Abständen ausgelesen und an die LoRaWAN-Gateways gesendet.


### Setup

#### Voraussetzung
Arduino IDE (Version 2.1.0)

#### Anleitung
1. Board installieren
   -`File` -> `Preferences...` -> `Additional boards manager URLs` -> Link einfügen:<br>
   https://github.com/HelTecAutomation/CubeCell-Arduino/releases/download/V1.5.0/package_CubeCell_index.json<br>
   ->`Boards Manager` öffnen und `CubeCell Development Framework` (aktuell: Version 1.5.0) installieren
2. Library installieren
   -`Library Manager` öffnen -> `Adafruit SHT31 Library` (aktuell: Version 2.2.0) und `Adafruit BusIO` (**Version 1.7.0** !!!) installieren (neuere Versionen funktionieren nicht)<br>
    - Beispiel: `File` -> `Examples` -> `Adafruit SHT31 Library` -> `SHT31test` 

## Kommunikation
Unser Projekt nutzt LoRaWAN, ein energiesparendes drahtloses Netzwerkprotokoll, um die Temperaturdaten von den Sensoren an das TTN zu übertragen. Für die beiden Projekte stehen in diesem Repo jeweils eine .ino Datei bereit, die es dem Mikrocontroller ermöglichen, den Abstandssensor auszulesen und die Daten über das LoRaWAN-Protokoll zu übertragen.

1. **lora-temperature.ino**

Es werden verschiedene Bibliotheken importiert, darunter die LoRaWAN_APP-Bibliothek und die Adafruit_SHT31-Bibliothek, die für die Kommunikation über das LoRaWAN-Protokoll und die Ansteuerung des SHT31-Sensors verwendet werden.

Der Code enthält auch Konfigurationsparameter für die LoRaWAN-Verbindung, wie die DevEUI, AppEUI, AppKey, nwkSKey, appSKey und devAddr. Diese Parameter werden verwendet, um das Gerät über die OTAA (Over-the-Air Activation) oder ABP (Activation by Personalization) mit dem LoRaWAN-Netzwerk zu verbinden.

Der Klimasensor liest die Batteriespannung, Luftfeuchtigkeit und Temperatur mithilfe des SHT31-Sensors aus. Die gelesenen Werte werden in einer Payload für den LoRaWAN-Funkübertragungsvorgang vorbereitet und gesendet. Der Sensor wird periodisch in einem Zyklus ausgelesen, der durch die Variable "appTxDutyCycle" definiert ist.

Der Code enthält auch Debugging-Funktionen, um den Status und die gelesenen Werte auf der seriellen Schnittstelle auszugeben. Die Schleife (loop) des Programms enthält einen Zustandsautomaten, der den Ablauf des Geräts steuert, einschließlich der Initialisierung, dem Beitritt zum LoRaWAN-Netzwerk, dem Senden von Sensorwerten und dem Ruhezustand.


2. **lora-distance.ino**

Zu Beginn des Codes werden die erforderlichen Bibliotheken und Konfigurationsparameter importiert, einschließlich der Geräte- und Anwendungsschlüssel für die OTAA- und ABP-Authentifizierung. Es werden auch Einstellungen für das LoraWan-Region und die Geräteklasse vorgenommen.

Der Code definiert dann die Variablen für den Ultraschallsensor, die Batterie und den Abstand. Der Sensor wird über eine SoftSerial-Verbindung initialisiert und konfiguriert. Es werden auch Debugging-Optionen und Pin-Einstellungen festgelegt.

Die Funktion "readSensor" wird verwendet, um die Sensorwerte zu lesen. Zunächst wird der Batteriestand ermittelt. Anschließend wird der Ultraschallsensor aktiviert, um den Abstand zu messen. Die gemessenen Daten werden verarbeitet und in die entsprechenden Variablen geschrieben.

Die Funktion "prepareTxFrame" bereitet das Datenpaket für die Übertragung vor. Es wird die Größe des Payloads festgelegt und die Batterie- und Abstandswerte in das Datenarray geschrieben.

Im Setup-Teil werden die serielle Kommunikation und der Vext-Pin konfiguriert. Der Ultraschallsensor wird initialisiert, und die LoRaWAN-Verbindung wird initialisiert und auf den Join-Zustand gesetzt.

In der Hauptschleife (loop) des Programms werden verschiedene Zustände behandelt. Im INIT-Zustand werden die Geräteparameter gedruckt und die LoRaWAN-Initialisierung durchgeführt. Im JOIN-Zustand wird versucht, sich mit dem Netzwerk zu verbinden. Im SEND-Zustand werden die Sensordaten gelesen und das Datenpaket vorbereitet. Es wird dann über LoRaWAN gesendet. Im CYCLE-Zustand wird der nächste Sendezyklus geplant, und im SLEEP-Zustand geht das Gerät in den Schlafmodus über.

Nach Abschluss eines Zustands wird je nach Ergebnis der vorherige Zustand erneut aufgerufen oder der INIT-Zustand gestartet, um den Zyklus zu wiederholen.




## Middleware

Die Middleware-Komponente wird universell für beide Projekte genutzt und umfasst eine Reihe von python-Dateien, die für die Kommunikation mit externen Diensten, die Datenverarbeitung und die Datenbankzugriffe verantwortlich sind.
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
Von der Open-Weather-API werden zwei Werte übertragen: Temperatur in Kelvin (wird später in Grad Celsius umgewandelt) und Luftfeuchtigkeit in Prozent.
get_weather_data(): Diese Funktion ruft Wetterdaten von der OpenWeatherMap-API ab. Sie verwendet die angegebenen Koordinaten, den API-Schlüssel und die URL, um eine Anfrage an die API zu senden. Die empfangenen Daten werden verarbeitet und mithilfe der insert_data_weather_api()-Funktion in die separate "weather_api"-Datenbank eingefügt.
on_interval(sc): Diese Funktion wird periodisch in einem festgelegten Intervall aufgerufen und ruft die get_weather_data()-Funktion auf, um aktuelle Wetterdaten abzurufen und einzufügen.
thread_function(): Diese Funktion wird als Hintergrundthread gestartet und ruft die on_interval()-Funktion in regelmäßigen Abständen auf, um Wetterdaten abzurufen.
init(): Diese Funktion initialisiert die Wetter-API-Integration. Sie startet den Hintergrundthread, der die Wetterdaten periodisch abruft und in die Datenbank einfügt.



## Datenbank

Für die Realisierung der Datenbank wurde InfluxDB aufgrund seiner Spezialisierung auf Zeitreihendaten und seiner nahtlosen Integration mit Grafana ausgewählt. 
Es ermöglicht die effiziente Speicherung, Abfrage und Visualisierung der erfassten Sensorwerte. Zudem skaliert es gut und bietet eine benutzerfreundliche Abfragesprache. InfluxDB erfüllt somit alle Anforderungen des Projekts.


### Datenbank-API

Die Datenbank mit den gemessenen Werten verfügt über eine API, die auf dem Endpunkt `http://<IP-Adresse>:5000` verfügbar ist. Es können von beiden Projekten Daten durch Aufrufen des Endpunkts `/` abgerufen werden. Optional können bestimmte Datenbereiche mithilfe von Unix-Zeitstempeln als Abfrageparameter angegeben werden. Diese Parameter werden als `first_time_point` und `latest_time_point` bezeichnet.
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
