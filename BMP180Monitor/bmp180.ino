#include <Adafruit_BMP085_U.h>

// atmospheric pressure in Pa, ambient temperature in C

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(18001);

void setup() {
  Serial.begin(115200);
  while (!bmp.begin()) {
    Serial.print("ERROR: no BMP085/180 detected!");
    delay(1000);
  }
}

void send_pt() {
  float P,T;
  bmp.getPressure(&P);
  bmp.getTemperature(&T);
  Serial.write((byte*)&P,4);
  Serial.write((byte*)&T,4);
}

void print_pt() {
  float P,T;
  bmp.getPressure(&P);
  bmp.getTemperature(&T);
  Serial.print(P);
  Serial.print(F(" hPa, "));
  Serial.print(T);
  Serial.print(F(" C"));
  Serial.println();
}

void loop() {
  if (Serial.available()>0) {
    int c = Serial.read();
    if (c=='q') send_pt();
    else if (c=='h') print_pt();
  }
}
