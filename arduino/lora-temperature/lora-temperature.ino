#include <Arduino.h>
#include <LoRaWan_APP.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

/* OTAA para*/
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0xEF, 0x9A };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0xC3, 0xC0, 0x27, 0x59, 0x89, 0x01, 0x7F, 0x95, 0x78, 0x33, 0x07, 0x08, 0x96, 0x94, 0xFB, 0x62 };

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
uint32_t appTxDutyCycle = 3 * 60000;  // 30 minutes

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

/* SHT31 Sensor */
Adafruit_SHT31 sht31 = Adafruit_SHT31();
bool send;
uint8_t battery;     // %
uint8_t humidity;    // %
int8_t temperature;  // C

/* debugging */
bool debug = false;

static void readSensor() {
  /* set error values */
  send = false;
  battery = 0b10000000;
  humidity = 0b10000000;
  temperature = 0b10000000;

  /* get battery percentage */
  battery = map(BoardGetBatteryLevel(), 1, 254, 0, 100);  // 0 and 255 of BoardGetBatteryLevel() are error values

  bool valid = false;
  int count = 0;
  do {
    /* activate power pin */
    digitalWrite(Vext, LOW);
    delay(2000);

    /* initialize sensor */
    sht31.begin();
    delay(1000);

    /* get humidity and temperature and override the error values */
    humidity = sht31.readHumidity();        // ignore decimal places
    temperature = sht31.readTemperature();  // ignore decimal places

    /* valid if no error values accure */
    valid = (humidity != 255 && temperature != -1);
    count++;

    if (debug) {
      Serial.print("try: ");
      Serial.print(count + 1);
      Serial.print(", valid: ");
      Serial.println(valid);
    }

    /* deactivate power pin */
    digitalWrite(Vext, HIGH);
    delay(1000);
  } while (!valid && count < 10);

  if (valid)
    send = true;

  if (debug) {
    /* print values */
    Serial.print("battery: ");
    Serial.print(battery);
    Serial.print("%, humidity: ");
    Serial.print(humidity);
    Serial.print("%, temperature: ");
    Serial.print(temperature);
    Serial.println("C");
  }
}

/* Prepares the payload of the frame */
static void prepareTxFrame(uint8_t port) {
  appDataSize = 3;
  appData[0] = battery;
  appData[1] = humidity;
  appData[2] = temperature;
}

void setup() {
  Serial.begin(115200);

  /* setup Vext pin */
  pinMode(Vext, OUTPUT);

  /* setup SHT31 sensor */
  // sht31.begin();

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
        readSensor();

        prepareTxFrame(appPort);
        if (send)
          LoRaWAN.send();

        deviceState = DEVICE_STATE_CYCLE;
        break;
      }
    case DEVICE_STATE_CYCLE:
      {
        // Schedule next packet transmission
        LoRaWAN.cycle(appTxDutyCycle);
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
