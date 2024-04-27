// ask_transmitter.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module




//#include <ros.h>

#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>
//#include<std_msgs/Float32.h>

#include <I2Cdev.h>
//HardwareSerial Serial3(PB10, PB11);

//ros::NodeHandle nh;
//std_msgs:: Float32 ogYaw;
//std_msgs:: Float32 filtYaw;
//ros::Publisher OG("OG", &ogYaw);
//ros::Publisher Filt("Filt", &filtYaw);

MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL
bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorInt16 aa;
VectorInt16 gy;
VectorInt16 aaReal;
VectorInt16 aaWorld;
VectorFloat gravity;
float euler[3];
float ypr[3];

//float X = 0, Q = 2.0, Un = 7, R = 1.0;
//float predX, predUn, K, Y;
//unsigned int Timer = 0;

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
uint8_t m1, m2, m3;
uint8_t virtualSwitch = 7;


RH_ASK driver;

void setup()
{
  Wire.begin();
  Wire.setClock(400000);
  //Serial3.begin(115200);
  Serial.begin(9600);
  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788);
  mpu.CalibrateAccel(6);
  mpu.CalibrateGyro(6);
  mpu.setDMPEnabled(true);
  dmpReady = true;
  packetSize = mpu.dmpGetFIFOPacketSize();
  Serial.begin(9600);	  // Debugging only
  if (!driver.init())
    Serial.println("init failed");
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(virtualSwitch, OUTPUT);
}

void loop()
{
  if (!dmpReady) return;
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    //Serial.println(ypr[0]* 180 / M_PI );
    //Serial.println((int)(ypr[1]* 180 / M_PI));
    //Serial.println((int)(ypr[2]* 180 / M_PI) );
  }
  m1 = digitalRead(4);
  m2 = digitalRead(5);
  m3 = digitalRead(6);
  uint16_t P,R;
  if (digitalRead(virtualSwitch) == HIGH) {
    Serial.println("l");
    P = (int)(((fabs(ypr[1]* 180 / M_PI))*255)/90);
    R = (int)(((fabs(ypr[2]* 180 / M_PI))*255)/90);
    Serial.println(P);
    Serial.println(R);
    uint8_t Fp, Fr;
    if (ypr[1] > 0)
      Fp = 0;
    else
      Fp = 1;
    if (ypr[2] > 0)
      Fr = 0;
    else
      Fr = 1;
      Serial.println(Fp);
      Serial.println(Fr);
    uint16_t msg[4] = {((P>10||P<0)? P:0), R , Fp, Fr };
    driver.send((uint8_t *)&msg, sizeof(msg));
    driver.waitPacketSent();
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    delay(200);
  }
  else {
    //delay(50);
    //if (!(m1 == digitalRead(4) && m2 == digitalRead(5) && m3 == digitalRead(6))) {

      //Serial.println("y"); HUMAN
      if (m1 == HIGH) {
        Serial.println("y");
        uint16_t msg[4] = {3, 0, 0, 0};
        driver.send((uint8_t*)&msg, sizeof(msg));
        driver.waitPacketSent();
        digitalWrite(A0, HIGH);
        digitalWrite(A1, LOW);
        digitalWrite(A2, LOW);
        delay(200);
      }
      else if (m2 == HIGH) {
        Serial.println("o");
        uint16_t msg[4] = {1, 0, 0, 0};
        driver.send((uint8_t*)&msg, sizeof(msg));
        driver.waitPacketSent();
        digitalWrite(A0, LOW);
        digitalWrite(A1, HIGH);
        digitalWrite(A2, LOW);
        delay(200);
      }
      else if (m3 == HIGH) {
        Serial.println("k");
        uint16_t msg[4] = {2, 0, 0, 0};
        driver.send((uint8_t*)&msg, sizeof(msg));
        driver.waitPacketSent();
        digitalWrite(A0, LOW);
        digitalWrite(A1, LOW);
        digitalWrite(A2, HIGH);
        delay(200);
      }
    //}
  }
}
