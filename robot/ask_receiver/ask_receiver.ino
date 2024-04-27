// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
uint16_t buf[4];
uint8_t buflen = sizeof(buf);
RH_ASK driver;


void setup()
{
  Serial.begin(9600);	// Debugging only
  if (!driver.init())
    Serial.println("init failed");
}

void loop()
{

  if (driver.recv((uint8_t*)&buf, &buflen)) {
    Serial.println(buf[0]);
    Serial.println(buf[1]);
    Serial.println(buf[2]);
    Serial.println(buf[3]);
  }
  delay(100);

  if (buf[0] == 1)
    Serial.println("OBS");

  else if (buf[0] == 2)
  Serial.println("FOL");
  else if (buf[0] == 3)
    Serial.println("LINE");

  else
    Serial.println("GES");

}
