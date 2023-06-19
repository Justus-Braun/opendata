# Dokumenation

## Inhaltsverzeichnis
1. [Projektdefintion und -umfeld](#projekt)
2. [Systemarchitektur](#architektur)
    1. [Hardware](#hardware)
        1. [Temperatursensor](#sensor)
        2. [Microcontroller](#microcontroller)
    3. [Kommunikation](#kommunikation)
        1. [LoraWan](#lorawan)
        2. [MQTT](#mqtt)
    4. [Middleware](#middleware)
    5. [Datenbank](#datenbank)
    6. [Visualisierung](#visualisierung)
3.[Anwendung](#anwendung)

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

## Hardware
##Temperatursensor
Eine zentrale Komponente unseres Systems ist der Sensor SKU:SEN0385.
Dieser Sensor ist in der Lage, sowohl die Temperatur als auch die Luftfeuchtigkeit präzise und zuverlässig zu messen.

Der Sensor SKU:SEN0385 ist speziell für die Erfassung von Umweltparametern entwickelt worden. 
Er verfügt über integrierte Messgeräte, die die Temperatur in Grad Celsius und die Luftfeuchtigkeit in Prozent messen können. 
Der Sensor arbeitet effizient und zeichnet sich durch eine hohe Genauigkeit und Stabilität aus.

Im Rahmen unseres Projekts ist der Sensor SKU:SEN0385 mit einem Microcontroller verbunden, der die Aufgabe hat, die Messwerte des Sensors zu erfassen und sie für die Übertragung über das LoRaWAN-Netzwerk vorzubereiten. Der Sensor wird regelmäßig ausgelesen, und die erfassten Temperatur- und Luftfeuchtigkeitswerte werden als wichtige Datenpunkte in unserem System verwendet.


##Microcontroller


## Kommunikation

Unser Projekt nutzt LoRaWAN, ein energiesparendes drahtloses Netzwerkprotokoll, um die Temperaturdaten von dem Sensor an das TTN zu übertragen.

LoRaWAN ermöglicht eine zuverlässige und weitreichende Kommunikation über große Entfernungen, was es ideal für IoT-Anwendungen macht.
