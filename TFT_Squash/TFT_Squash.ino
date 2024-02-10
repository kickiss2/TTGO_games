#define BLACK 0x0000
#define WHITE 0xFFFF

#include <TFT_eSPI.h>  // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

int16_t h = 130;
int16_t w = 235;

int16_t lbut = 0;
int16_t rbut = 35;

int16_t ipady = 0;
int16_t pady = 0;
int16_t padh = 20;

int dly = 5;
int16_t s = 6;

int16_t x = 0;
int16_t y = 0;
int16_t dx = 1;
int16_t dy = 1;

int16_t inertia = 0;
int16_t ispeed = 130;

void setup(void) {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);
  pinMode(lbut, INPUT_PULLUP);
  pinMode(rbut, INPUT_PULLUP);
  resetBall();
}

void loop() {
  delay(dly);
  paddle();
  ball();
}

void resetBall() {
  x = 0;
  y = random(0, h);
  dx = 1;
  dy = random(1, 3) - 1;
}

void ball() {
  if (x + s > w + 200) {
    resetBall();
  }

  tft.drawRect(x, y, s, s, BLACK);
  x = x + dx;
  y = y + dy;
  tft.fillRect(x, y, s, s, WHITE);

  if (y + s == h || y == 0) {
    dy = -dy;
  }

  if (x == 0) {
    dx = -dx;
  }

  if (x + s == w) {
    if (y >= pady && y <= pady + padh) {
      dx = -dx;
    }
    if (y <= pady + padh && y >= (pady + padh / 2) && dy <= 0) {
      dy = 1;
    }
    if (y >= pady && y <= (pady + padh / 2) && dy >= 0) {
      dy = -1;
    }
  }
}

void paddle() {
  tft.drawRect(w - 4, pady, 4, padh, BLACK);
  if (digitalRead(lbut) == 0) {
    inertia = ispeed;
  }
  if (digitalRead(rbut) == 0) {
    inertia = -ispeed;
  }
  if (digitalRead(lbut) == 0 && digitalRead(rbut) == 0) {
    inertia = 0;
  }
  if (inertia > 0 and pady + padh < h) {
    ipady = ipady + inertia;
    pady = round(ipady/ispeed);
    inertia = inertia - 1;
  }
  if (inertia < 0 and pady > 0) {
    ipady = ipady + inertia;
    pady = round(ipady/ispeed);
    inertia = inertia + 1;
  }
  tft.drawRect(w - 4, pady, 4, padh, WHITE);
}
