#include <Arduino.h>
#include <LoRaWan_APP.h>
#include <softSerial.h>

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

/* OTAA para*/
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0xEF, 0x8B };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0xFF, 0x69, 0xD1, 0x97, 0x51, 0x51, 0x40, 0x9F, 0xEA, 0xDA, 0x88, 0xC6, 0x07, 0x8A, 0x46, 0x6B };

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
uint32_t appTxDutyCycle = 10 * 60000;  // 10 minutes

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

/* A01NYUB ultrasonic sensor */
softSerial sensorSerial(GPIO0, GPIO5);  // RX, TX
unsigned char data[4] = {};
uint8_t battery;    // %
uint16_t distance;  // cm

/* debugging */
bool debug = false;

static void readSensor() {
  /* set error values */
  battery = 0b10000000;
  distance = 0;

  /* get battery percentage */
  battery = map(BoardGetBatteryLevel(), 1, 254, 0, 100);  // 0 and 255 of BoardGetBatteryLevel() are error values

  /* activate power pin */
  digitalWrite(Vext, LOW);
  delay(2000);

  do {
    delay(150);

    /* read distance sensor */
    do {
      for (int i = 0; i < 4; i++) {
        data[i] = sensorSerial.read();
      }
    } while (sensorSerial.read() == 0xff);

    sensorSerial.flush();
  } while (data[0] != 0xff);

  /* deactivate power pin */
  digitalWrite(Vext, HIGH);
  delay(1000);

  int sum;
  sum = (data[0] + data[1] + data[2]) & 0x00FF;
  if (sum == data[3]) {
    distance = ((data[1] << 8) + data[2]) / 10;
    if (distance <= 28)
      distance = 0;
  }

  if (debug) {
    /* print values */
    Serial.print("battery: ");
    Serial.print(battery);
    Serial.print("%, distance: ");
    Serial.print(distance);
    Serial.println("cm");
    delay(10);
  }
}

/* Prepares the payload of the frame */
static void prepareTxFrame(uint8_t port) {
  appDataSize = 3;
  appData[0] = battery;
  appData[1] = distance >> 8;
  appData[2] = distance;
}

void setup() {
  Serial.begin(115200);

  /* setup Vext pin */
  pinMode(Vext, OUTPUT);

  /* setup ultrasonic sensor */
  sensorSerial.begin(9600);

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
