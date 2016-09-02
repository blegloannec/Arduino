
#define cs   10
#define dc   8
#define rst  -1  // you can also connect this to the Arduino reset

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define H (tft.height())
#define W (tft.width())

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

void TitleScreen() {
  tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(23, 45);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(5);
  tft.println("GNOP");
}

void setup(void) {
  //Serial.begin(9600);
  tft.initR(INITR_BLACKTAB); 
  tft.setRotation(tft.getRotation()-1);
  TitleScreen();
  tft.setRotation(tft.getRotation()+1);
  randomSeed(analogRead(0));
  delay(2000);
}

void Flash() {
  for (int i=0; i<3; ++i) {
    tft.invertDisplay(true);
    delay(75);
    tft.invertDisplay(false);
    delay(75);
  }
}

#define BUTTON_NONE 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_SELECT 3
#define BUTTON_UP 4
#define BUTTON_LEFT 5

int ReadButton(void) {
  float a = analogRead(3);
  a *= 5.0;
  a /= 1024.0;
  if (a < 0.2) return BUTTON_DOWN;
  if (a < 1.0) return BUTTON_RIGHT;
  if (a < 1.5) return BUTTON_SELECT;
  if (a < 2.0) return BUTTON_UP;
  if (a < 3.2) return BUTTON_LEFT;
  else return BUTTON_NONE;
}

#define C 4
#define B 25
#define E 4

void PlayGame() {
  int ox = 0;
  int x = random(W-C);
  int y = 2;
  int dx = random(2)*2-1;
  int dy = 1;
  int sx = W/2-B/2;
  tft.fillScreen(ST7735_BLACK);
  tft.fillRect(sx, H-C-E, B, C, ST7735_WHITE);
  while (true) {
    ox = sx;
    if (x+C>W || x<0) dx = -dx;
    if (y<0) dy = -dy;
    if (H-C-E<=y+C && y+C<=H-C/2-E && sx<x && x<sx+B) dy = -dy;
    tft.fillRect(x, y, C, C, ST7735_BLACK);
    x += dx;
    y += dy;
    int b = ReadButton();
    if (b==BUTTON_RIGHT && sx+B<W) sx += 2;
    else if (b == BUTTON_LEFT && sx>=0) sx -= 2;
    tft.fillRect(x, y, C, C, ST7735_WHITE);
    if (sx!=ox) {
      tft.fillRect(ox, H-C-E, B, C, ST7735_BLACK);
      tft.fillRect(sx, H-C-E, B, C, ST7735_WHITE);
    }
    delay(15);
    if (y+C>=H) return;
  }
}

void loop() {
  PlayGame();
  Flash();
}
