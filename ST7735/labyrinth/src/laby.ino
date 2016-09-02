
#define cs   10
#define dc   8
#define rst  -1  // you can also connect this to the Arduino reset

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define H (tft.height())
#define W (tft.width())

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

void WaitScreen(void) {
  tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(5,5);
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(5);
  tft.println("+");
}

#define MAXX 21
#define MAXY 21
#define NUM(X,Y) ((X)*MAXY+(Y))
boolean L[MAXX][MAXY][2];
int tarjan[MAXX*MAXY];

int Find(int n) {
  if (tarjan[n]!=n) {
    int r = Find(tarjan[n]);
    tarjan[n] = r;
  }
  return tarjan[n];
}

boolean Union(int a, int b) {
  int fa = Find(a);
  int fb = Find(b);
  if (fa!=fb) {
    tarjan[fa] = fb;
    return true;
  }
  return false;
}

void Gen(void) {
  for (int i=0; i<MAXX*MAXY; ++i) tarjan[i] = i;
  for (int x=0; x<MAXX; ++x)
    for (int y=0; y<MAXY; ++y)
      for (int k=0; k<2; ++k)
        L[x][y][k] = true;
  int nb = MAXX*MAXY;
  while (nb>1) {
    int x = random(MAXX);
    int y = random(MAXY);
    int k = random(2);
    if (L[x][y][k]) {
       if (k==0 && x<MAXX-1 && Union(NUM(x,y),NUM(x+1,y))) {
         L[x][y][k] = false;
         --nb;
       }
       else if (k==1 && y<MAXY-1 && Union(NUM(x,y),NUM(x,y+1))) {
         L[x][y][k] = false;
         --nb;
       }
    }
  }
}

#define C 6

void Affiche(void) {
  tft.fillRect(0,0,C*MAXX,C*MAXY,ST7735_WHITE);
  for (int x=0; x<MAXX; ++x) {
    for (int y=0; y<MAXY; ++y) {
      if (L[x][y][0])
        tft.fillRect(C*(x+1)-1,C*y,1,C,ST7735_BLACK);
      if (L[x][y][1])
        tft.fillRect(C*x,C*(y+1)-1,C,1,ST7735_BLACK);
    }
  }
}

void setup(void) {
  //Serial.begin(9600);
  tft.initR(INITR_BLACKTAB); 
  randomSeed(analogRead(0));
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

void PlayGame() {
  int ox,oy;
  int x = 0;
  int y = 0;
  tft.fillRect(C*x+1,C*y+1,C-3,C-3,ST7735_RED);
  tft.fillRect(C*(MAXX-1),C*(MAXY-1),C-1,C-1,ST7735_BLUE);
  while (true) {
    ox = x;
    oy = y;
    int b = ReadButton();
    if (b==BUTTON_RIGHT && x<MAXX-1 && !L[x][y][0]) ++x;
    else if (b==BUTTON_LEFT && x>0 && !L[x-1][y][0]) --x;
    else if (b==BUTTON_UP && y>0 && !L[x][y-1][1]) --y;
    else if (b==BUTTON_DOWN && y<MAXY-1 && !L[x][y][1]) ++y;
    if (x!=ox || y!=oy) {
      tft.fillRect(C*ox+1,C*oy+1,C-3,C-3,ST7735_WHITE);
      tft.fillRect(C*x+1,C*y+1,C-3,C-3,ST7735_RED);
    }
    delay(200);
    if (x==MAXX-1 && y==MAXY-1) return;
  }
}

void loop() {
  WaitScreen();
  Gen();
  Affiche();
  PlayGame();
}
