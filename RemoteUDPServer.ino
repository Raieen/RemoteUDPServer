/*
 * RemoteUDP Server
 * 
 * Send hex values to be emitted by the IR emitter. * 
 * 
 */
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <IRremote.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);
unsigned int port = 6969;

char incomingBuffer[UDP_TX_PACKET_MAX_SIZE]; // Buffer for incoming packet.
EthernetUDP udp;

int emitterPin = 3;
IRsend irsend;

void setup() {
  Ethernet.init(10); // Init the ethernet shield
  pinMode(emitterPin, OUTPUT);
  Serial.begin(9600);
  while(!Serial) {
    // Wait  
  }

  Ethernet.begin(mac, ip);

  // Check ethernet is setup.
  if(Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet Shield not found.");
    while(true) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(2000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(2000);
    }
  }

  if(Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable not found.");
    while(true) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(2000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(2000);
    }
  }

  udp.begin(port);
  Serial.println("Started UDP Server");    
}

void loop() {
  int packetSize = udp.parsePacket();
  memset(incomingBuffer, 0, sizeof(incomingBuffer));
  if(packetSize) {
    Serial.print("Packet Size - ");
    Serial.println(packetSize);
    IPAddress address = udp.remoteIP();

    udp.read(incomingBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents");
    Serial.println(incomingBuffer);

    int hexNum = (int) strtol(incomingBuffer, NULL, 16);
//    Serial.print("Hex: ");
//    Serial.println(hexNum, HEX);
//    Serial.println((int) strtol("A90", NULL, 16), HEX);
//    Serial.println(0xa90, HEX);
    
    irsend.sendNEC(hexNum, 32); // Maybe sendSony() or sendNEC()
    // This used to also power the light but doesn't now...
//    digitalWrite(emitterPin, HIGH);
//    delay(100);
//    digitalWrite(emitterPin, LOW);
  }
}
