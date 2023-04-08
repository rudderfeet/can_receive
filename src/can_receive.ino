// Based on Copyrighted code from (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>

#define STARTUP_PAUSE 1000
#define CAN_PAUSE 1000
#define ONBOARD_LED_BLINK_PAUSE_MS 100
#define ONBOARD_LED_PIN  2
#define CAN_TX_PIN 33
#define CAN_RX_PIN 32

void setup() {

  // Initialize the serial console and wait a second to start
  Serial.begin(115200);
  delay(STARTUP_PAUSE);

  // Tell the serial console who we are
  Serial.println("****************");
  Serial.println("* CAN Receiver *");
  Serial.println("****************");
  
  // Initialize the onboard LED
  pinMode(ONBOARD_LED_PIN,OUTPUT);

  // Specify which hardware pins to use for RX and TX
  // (Default is 4 and 5)
  CAN.setPins(CAN_RX_PIN, CAN_TX_PIN);

  // Start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  } else {
    Serial.println("CAN Successfully Started.");
  }

}

void loop() {

  // try to parse packet
  int packetSize = CAN.parsePacket();

  if (packetSize) {
    // received a packet
    Serial.print ("Received ");
    blink();

    if (CAN.packetExtended()) {
      Serial.print ("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print ("RTR ");
    }

    Serial.print ("packet with id 0x");
    Serial.print (CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print (" and requested length ");
      Serial.println (CAN.packetDlc());
    } else {
      Serial.print (" and length ");
      Serial.println (packetSize);

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print ((char) CAN.read());
      }
      Serial.println();
    }

    Serial.println();
  }
}

void blink() {
  // Blink the onboard LED
  digitalWrite(ONBOARD_LED_PIN,HIGH);
  delay(ONBOARD_LED_BLINK_PAUSE_MS);
  digitalWrite(ONBOARD_LED_PIN,LOW);
}