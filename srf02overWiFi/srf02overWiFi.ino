/*
  read cm from SRF02 and returns to local host
*/

#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <Wire.h>

int reading=0;
BridgeServer server;
// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
BridgeClient client;

void setup() {
  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Bridge.begin();
  digitalWrite(13, LOW);
  Wire.begin();
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  client = server.accept();
  if (client) {
    String command = client.readStringUntil('\r');
    if (command == "srf") {
      Wire.beginTransmission(112); // transmit to device #112 (0x70)
      Wire.write(byte(0x00));      // sets register pointer to the command register (0x00)
      Wire.write(byte(0x51));      // command sensor to measure in "centimeters" (0x51)
      Wire.endTransmission();      // stop transmitting
      delay(70);                   // datasheet suggests at least 65 milliseconds
      Wire.beginTransmission(112); // transmit to device #112
      Wire.write(byte(0x02));      // sets register pointer to echo #1 register (0x02)
      Wire.endTransmission();      // stop transmitting
      Wire.requestFrom(112, 2);    // request 2 bytes from slave device #112
      }
    if (command == "test") {
      client.println("TEST");
      digitalWrite(13, HIGH);
      delay(500);
      digitalWrite(13, LOW);}
  }
  if (2 <= Wire.available()){
    reading = Wire.read();
    reading = reading << 8;
    reading |= Wire.read();
    client.println(reading);
  }
  client.stop();  
  delay(50); // Poll every 50ms
}

