//#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);

void setup() {
  Serial.begin(115200);
  while (!accel.begin()) {
    //Serial.println(F("Error: no LSM303 detected"));
    delay(1000);
  }
  while (!mag.begin()) {
    //Serial.println("Error: no LSM303 detected");
    delay(1000);
  }
  while (!gyro.begin()) {
    //Serial.print("Error: no L3GD20 detected");
    delay(1000);
  }
}

void send_float(float *x) {
  Serial.write((char*)x,4);
}

void send_accel() {
  sensors_event_t event;
  accel.getEvent(&event);
  send_float(&event.acceleration.x);
  send_float(&event.acceleration.y);
  send_float(&event.acceleration.z);
}

void send_mag() {
  sensors_event_t event;
  mag.getEvent(&event);
  send_float(&event.magnetic.x);
  send_float(&event.magnetic.y);
  send_float(&event.magnetic.z);
}

void send_gyro() {
  sensors_event_t event;
  gyro.getEvent(&event);
  send_float(&event.gyro.x);
  send_float(&event.gyro.y);
  send_float(&event.gyro.z);
}

void loop() {
  if (Serial.available()>0) {
    char c = Serial.read();
    if (c=='a') send_accel();
    else if (c=='m') send_mag();
    else if (c=='g') send_gyro();
  }
  //delay(10);
}
