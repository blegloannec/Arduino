#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define cs   10
#define dc   8
#define rst  -1

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

#define H 160
#define L 128
#define S 11
#define COL0 ST7735_WHITE
#define COL1 ST7735_BLACK

int M[2][L];
int rule;

int val2col(int v) {
   if (v==0) return COL0;
   else return COL1;
}

void setup(void) {
  //Serial.begin(9600);
  tft.initR(INITR_REDTAB);
  randomSeed(analogRead(0));
}


void loop() {
  int curr, next;
  tft.fillScreen(COL1);
  
  rule = random(256);
  // Serial.println(rule);
  tft.setTextColor(ST7735_RED);
  tft.setCursor(0,153);
  tft.print(rule);
  curr = 0;
  for (int i=0; i<L; ++i) {
    M[curr][i] = random(2);
    tft.drawPixel(i,0,val2col(M[curr][i]));
  }
  for (int j=1; j<H; ++j) {
    for (int i=0; i<L; ++i) {
      next = (curr+1)%2;
      M[next][i] = (rule>>(4*(M[curr][(i+L-1)%L])+2*(M[curr][i])+(M[curr][(i+1)%L])))&1;
      tft.drawPixel(i,j,val2col(M[next][i]));
    }
    curr = next;
  }
  
  delay(1000);
}
