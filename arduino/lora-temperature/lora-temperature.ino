#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>
#include "Adafruit_SHT31.h"

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

/* OTAA para*/
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0xDA, 0x42 };  // 70B3D57ED005DA42
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x91, 0x25, 0xB0, 0xB3, 0xE3, 0x37, 0x69, 0xAA, 0x76, 0x24, 0x0E, 0x22, 0xB8, 0xB5, 0xFA, 0xBA };  // 9125B0B3E33769AA76240E22B8B5FABA

/* ABP para*/
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda, 0x85 };
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef, 0x67 };
uint32_t devAddr = (uint32_t)0x007e6ae1;

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

/*LoraWan region*/
LoRaMacRegion_t loraWanRegion = LORAMAC_REGION_EU868;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15000;

/*OTAA or ABP*/
bool overTheAirActivation = true;

/*ADR enable*/
bool loraWanAdr = true;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = false;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = false;

/* Application port */
uint8_t appPort = 2;

/* Number of trials to transmit the frame */
uint8_t confirmedNbTrials = 4;

// SHT31 Sensor
Adafruit_SHT31 sht31 = Adafruit_SHT31();

bool sensorActive = false;
int last_sent_battery = -9999;      // error value
int last_sent_humidity = -9999;     // error value
int last_sent_temperature = -9999;  // error value

static int* readSensor() {
  // TODO: activate power pin
  // TODO: read battery percentage (as int)

  int data[4] = { 0, -9999, -9999, -9999 };  // send (0 or 1), battery, humidity, temperature

  data[1] = 69;  // TODO: data[1] = get_battery_%
  // data[1] = getBatteryVoltage();

  if (sensorActive) {  // if the sensor is active we override the error values
    // TODO: activate power pin
    // TODO: initialize sensor?!
    data[2] = sht31.readHumidity() * 100;
    data[3] = sht31.readTemperature() * 100;
    // TODO: deactivate power pin
  }

  // just send if a value has changed
  if (last_sent_battery != data[1]) {
    last_sent_battery = data[1];
    data[0] = 1;
  }
  if (last_sent_humidity != data[2]) {
    last_sent_humidity = data[2];
    data[0] = 1;
  }
  if (last_sent_temperature != data[3]) {
    last_sent_temperature = data[3];
    data[0] = 1;
  }

  return data;
}

/* Prepares the payload of the frame */
static void prepareTxFrame(uint8_t port, int battery, int humidity, int temperature) {
  appDataSize = 5;
  appData[0] = battery;           // battery
  appData[1] = humidity >> 8;     // humidity
  appData[2] = humidity;          // humidity
  appData[3] = temperature >> 8;  // temperature
  appData[4] = temperature;       // temperature
}


void setup() {
  Serial.begin(115200);

  // Setup SHT31
  if (sht31.begin())
    sensorActive = true;  // SHT31 ready
                          // else: Couldn't find SHT31

  // Serial.print("Heater Enabled State: ");
  // if (sht31.isHeaterEnabled())
  //   Serial.println("ENABLED");
  // else
  //   Serial.println("DISABLED");

  // #if(AT_SUPPORT)
  // 	enableAt();
  // #endif
  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();
}

void loop() {
  switch (deviceState) {
    case DEVICE_STATE_INIT:
      {
        // #if(LORAWAN_DEVEUI_AUTO)
        // 			LoRaWAN.generateDeveuiByChipID();
        // #endif
        // #if(AT_SUPPORT)
        // 			getDevParam();
        // #endif
        printDevParam();
        LoRaWAN.init(loraWanClass, loraWanRegion);
        deviceState = DEVICE_STATE_JOIN;
        break;
      }
    case DEVICE_STATE_JOIN:
      {
        LoRaWAN.join();
        break;
      }
    case DEVICE_STATE_SEND:
      {
        int* data = readSensor();
        prepareTxFrame(appPort, data[1], data[2], data[3]);
        if (1 == data[0]) {  // if the data should be send
          LoRaWAN.send();
        }
        deviceState = DEVICE_STATE_CYCLE;
        break;
      }
    case DEVICE_STATE_CYCLE:
      {
        // Schedule next packet transmission
        txDutyCycleTime = appTxDutyCycle + randr(0, APP_TX_DUTYCYCLE_RND);
        LoRaWAN.cycle(txDutyCycleTime);
        deviceState = DEVICE_STATE_SLEEP;
        break;
      }
    case DEVICE_STATE_SLEEP:
      {
        LoRaWAN.sleep();
        break;
      }
    default:
      {
        deviceState = DEVICE_STATE_INIT;
        break;
      }
  }
}
